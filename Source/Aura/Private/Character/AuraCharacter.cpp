// copyright


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	//相机弹簧臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	//绝对旋转，角色旋转时不会跟随角色旋转
	CameraBoom->SetUsingAbsoluteRotation(true);
	//关闭弹簧臂和角色产生碰撞
	CameraBoom->bDoCollisionTest = false;

	//相机
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	//连接弹簧臂末端插槽
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//关闭使用Pawn控制旋转
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	//升级奶瓜特效组件
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	//升级奶瓜特效默认不会自动激活
	LevelUpNiagaraComponent->bAutoActivate = false;
	
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

	//玩家默认职业
	CharacterClass = ECharacterClass::Elementalist;
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

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//通过XP查找当前等级
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetPlayerXP_Implementation() const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取当前经验值
	return AuraPlayerState->GetPlayerXP();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取要奖励多少属性点
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取要奖励多少技能点
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取可用属性点
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	//获取玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取可用技能点
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	//获取玩家状态
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//增加玩家经验
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	//获取玩家状态
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//增加玩家等级
	AuraPlayerState->AddToLevel(InPlayerLevel);

	//满足技能的玩家等级要求更新技能状态
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatus(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	//获取玩家状态
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//增加可用属性点
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPointsReward)
{
	//获取玩家状态
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//增加可用技能点
	AuraPlayerState->AddToSpellPoints(InSpellPointsReward);
}

void AAuraCharacter::LevelUp_Implementation()
{
	//多播升级粒子特效
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		//获取相机位置
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		//获取奶瓜粒子特效位置
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		//计算旋转 粒子和相机朝向平行
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		//设置粒子特效世界坐标旋转
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		//激活
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	//检查玩家状态
	const AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//获取玩家等级
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	//初始化ability  actor info
	AAuraPlayerState* AuraPlayerState =  GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	//设置玩家状态为OwnerActor
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
