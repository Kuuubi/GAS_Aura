// copyright


#include "Character/AuraEnemy.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	//设置碰撞预设
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//ASC
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	//开启复制
	AbilitySystemComponent->SetIsReplicated(true);
	//复制模式
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//AS
	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");
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

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	//初始化ability actor info
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}
