// copyright


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	//启用复制
	bReplicates = true;

	//样条曲线
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//映射上下文判空
	check(AuraContext);

	//从本地角色上获取它的子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//子系统判空
	//不能使用check(Subsystem)，否则多人游戏中有其他玩家，游戏会崩溃
	//在多人游戏中，如果有其他玩家，此时在本机初始化其他玩家的时候，Subsystem为空
	if (Subsystem)
	{
		//添加映射上下文，优先级为0，可以添加多个
		Subsystem->AddMappingContext(AuraContext, 0);
	}


	//显示鼠标光标
	bShowMouseCursor = true;
	//设置鼠标样式为默认样式
	DefaultMouseCursor = EMouseCursor::Default;

	//输入模式，控制游戏内逻辑和UI
	FInputModeGameAndUI InputModeData;
	//不锁定鼠标
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//鼠标按下时不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	//应用玩家控制器输入模式
	SetInputMode(InputModeData);
}

//输入组件
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//断言类型转换，将输入组件强转为增强输入组件
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	//绑定动作事件，MoveAction输入动作，一直按下时调用Move函数
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	//ShiftAction，按住时始终触发
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	//绑定能力输入操作
	AuraInputComponent->BindAbilityActions(InputConfig, this,&ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

//PlayerTick
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//鼠标跟踪
	CursorTrace();
	
	//自动寻路
	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		//创建伤害文本组件
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		//动态创建的组件需要调用注册组件
		DamageText->RegisterComponent();
		//将伤害文本组件添加到目标角色根组件上，并保持变换
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		//伤害文本和目标角色分离，不跟随角色移动
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//设置伤害文本
		DamageText->SetDamageText(DamageAmount);
	}
}

//高亮敌人轮廓
//鼠标跟踪
void AAuraPlayerController::CursorTrace()
{
	//返回鼠标下命中
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//是否命中被阻挡
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	//判断是否实现敌人接口
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActoer();
		if (ThisActor) ThisActor->HighlightActoer();
	}
}

//绑定输入回调函数
//短按
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//鼠标左键标签
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//是否正在瞄准敌人
		bTargeting = ThisActor ? true : false;
		//自动寻路
		bAutoRunning = false;
		//重置长按时间
		FollowTime = 0.f;
	}

}

//释放
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//如果不是鼠标左键标签
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//输入被按下
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	
	//瞄准敌人
	if (GetASC())	GetASC()->AbilityInputTagReleased(InputTag);
	
	//实现自动寻路
	if (!bTargeting && !bShiftKeyDown)
	{
		//输入被按下
		if (GetASC())	GetASC()->AbilityInputTagReleased(InputTag);
		{
			//获取Pawn
			const APawn* ControlledPawn = GetPawn();
			//短按
			if (FollowTime <= ShortPressThreshold && ControlledPawn)
			{
				//创建导航路径
				if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
				{
					//样条曲线
					//清除样条内现有的点
					Spline->ClearSplinePoints();
					//遍历路径点数组
					for (const FVector& PointLoc : NavPath->PathPoints)
					{
						//样条曲线添加点
						Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
						//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Orange, false, 5.f);
					}
					//将路径最后一个点位设置成目的地
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					//自动寻路
					bAutoRunning = true;
				}
			}
			//重置长按时间
			FollowTime = 0.f;
			//重置是否选择目标
			bTargeting = false;
		}
	}
}

//长按
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//如果不是鼠标左键标签
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//输入被按下
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	
	//瞄准敌人
	if (bTargeting || bShiftKeyDown)
	{
		//输入被按下
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else	//实现左键长按移动
	{
		//自增按下多久时间
		FollowTime += GetWorld()->GetDeltaSeconds();

		//鼠标光标射线跟踪命中结果
		//缓存击中点位置
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		//朝向移动
		//获取有效被占用的Pawn
		if (APawn* ControllerPawn = GetPawn())
		{
			//计算移动方向
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			//移动
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

//获取ASC
UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent ==nullptr)
	{
		//用蓝图库从Pawn获取ASC
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	
	//自动寻路
	//获取有效Pawn
	if (APawn*  ControlledPawn = GetPawn())
	{
		//Pawn离样条线最近的点位置
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		//找到最接近线条世界位置的方向
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		//朝指定方向移动
		ControlledPawn->AddMovementInput(Direction);
		//检查是否到到达线条点位置
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

//键盘移动
void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	//获取输入操作的2d向量值
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2d>();
	//获取控制器旋转
	const FRotator Rotation = GetControlRotation();
	//Y绿轴Pitch俯仰角，Z蓝轴Yaw偏航角，X红轴Roll滚动角
	//只留偏航角，人在原地旋转
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
	//获取向前的值
	const FVector ForwardDirection =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//获取向右的值
	const FVector RighterDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn =  GetPawn<APawn>())
	{
		//WS按键
		ControllerPawn->AddMovementInput(ForwardDirection , InputAxisVector.Y);
		//AD按键
		ControllerPawn->AddMovementInput(RighterDirection , InputAxisVector.X);
	};
}

