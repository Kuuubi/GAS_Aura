// copyright


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	//当设置为True时，角色将朝移动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement =true;
	//控制角色旋转速率
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//当设置为True时，角色移动约束在平面
	GetCharacterMovement()->bConstrainToPlane = true;
	//设置了上面一项为True，此项设置为True
	//则游戏开始时，角色被吸附到地面，防止有空中坠落，或卡在地面的问题
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//禁止相机随角色旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//服务器初始化ability  actor info
	InitAbilityActorInfo();
	//添加角色能力
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//客户端初始化ability  actor info
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	//检查玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check((AuraPlayerState));
	//获取玩家等级
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	//初始化ability  actor info
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check((AuraPlayerState));
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	//转换初始化完成后调用函数
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	//设置ASC，AS指针
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState ->GetAttributeSet();

	//获取有效玩家控制器
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		//获取有效HUD
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			//调用初始化Overlay
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent,AttributeSet);
		}
	}

	//初始化游戏效果于属性
	InitializeDefaultAttribute();
	
}
