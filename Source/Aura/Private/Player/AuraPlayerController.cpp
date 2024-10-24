// copyright


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	check(Subsystem);
	//添加映射上下文，优先级为0，可以添加多个
	Subsystem->AddMappingContext(AuraContext, 0);

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
