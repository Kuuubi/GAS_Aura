// copyright


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	// 不自动激活奶瓜
	bAutoActivate = false;
	
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()); // 获取到战斗接口
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC) // ASC初始化完成
	{
		// 监听负面标签变动回调
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface) // 如果绑定时，ASC未初始化成功，则监听ASC创建完成委托，完成对负面标签的监听
	{
		// AddWeakLambda 这种绑定方式的主要好处是当绑定的对象被销毁时，委托不会保持对象的引用，从而避免悬空指针问题和内存泄漏
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			// 监听负面标签变动回调
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
	if (CombatInterface)
	{
		// 角色死亡时
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());

	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();	// 激活
	}else
	{
		Deactivate(); // 停止
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeathActor)
{
	Deactivate(); // 停止
}
