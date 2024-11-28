// copyright


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	//设置碰撞预设
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//ASC
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	//开启复制
	AbilitySystemComponent->SetIsReplicated(true);
	//复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//AS
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	//血量条
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	//添加到根组件
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI角色只会由服务器控制，所以AIController只会在服务器端执行的
	if (!HasAuthority()) return;
	//AI控制器转换成AuraAIController
	AuraAIController = Cast<AAuraAIController>(NewController);

	//初始化行为树上设置的黑板
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	//运行行为树
	AuraAIController->RunBehaviorTree(BehaviorTree);
}

void AAuraEnemy::HighlightActoer()
{
	//自定义深度通道
	//网格体
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	//武器
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActoer()
{
	//自定义深度通道
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

//获取敌人等级
int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	//到时间后会将角色销毁
	SetLifeSpan(LifeSpan);
	Super::Die();
}

//受击反馈回调
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	//设置移动速度
	GetCharacterMovement()->MaxWalkSpeed =  BaseWalkSpeed;
	//初始化ActorInfo
	InitAbilityActorInfo();
	
	if (HasAuthority())
	{
		//赋予初始GA
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	
	
	//设置Widget控制器
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	//属性值变化时广播新值
	if (const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
		);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
					[this](const FOnAttributeChangeData& Data)
					{
						OnMaxHealthChanged.Broadcast(Data.NewValue);
					}
			);

		//监听标签变化
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		//广播血量初始值
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
	
}

void AAuraEnemy::InitAbilityActorInfo()
{
	//初始化ability actor info
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	//转换初始化完成后调用函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		//初始化属性
		InitializeDefaultAttribute();
	}

}

void AAuraEnemy::InitializeDefaultAttribute() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
