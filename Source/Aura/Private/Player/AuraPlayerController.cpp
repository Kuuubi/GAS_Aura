// copyright


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//启用复制
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//映射上下文判空
	check(AuraContext);

	//从本地角色上获取它的子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//子系统判空
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

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//断言类型转换，将输入组件强转为增强输入组件
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	//绑定动作事件，MoveAction输入动作，一直按下时调用Move函数
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered,this, &AAuraPlayerController::Move);
}

//PlayerTick
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

//鼠标跟踪
void AAuraPlayerController::CursorTrace()
{
	//返回鼠标下命中
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	//是否命中被阻挡
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	//判断是否实现敌人接口，如果没实现，就会返回一个空指针
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	*	从鼠标光标跟踪开始的几种情况
	*	A.	LastActor is null && ThisActor is null 都为空指针
	*			-	鼠标光标下不是敌人，可能是墙或者地板或其他没有实现敌人接口，这种情况声明都不做
	*	B.	LastActor is null && ThisActor is vaild 之前的actor是空的，现在的actor是有效的
	*			-	鼠标光标从墙上滑动到敌人身上，要高亮这个敌人，Higliht ThisActor
	*	C.	LastActor is vaild && ThisActor is null 之前的actor是有效的，现在是空的
	*			-	鼠标从敌人滑动到墙上，要把敌人关闭高亮，UnHigliht LastActor
	*	D.	两个都是is vaild，但是LastActor != ThisActor，他们不是同一个Actor
	*			 -  鼠标光标从一个敌人滑动到另个敌人身上，UnHigliht LastActor，Higliht ThisActor
	*	E.		两个都是is vaild，并且LastActor  = ThisActor，他们是同一个Actor
	*			-	鼠标一直在这个敌人身上，不需要取消光亮，什么都不做
	*/  
	
	if(LastActor == nullptr)
	{
		if(ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActoer();
		}
		else
		{
			// Case A - 全部空指针
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActoer();
		}
		else // 两个都有效
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor -> UnHighlightActoer();
				ThisActor->HighlightActoer();
			}
			else 
			{
				// Case E
			}
		}
	}
}

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

