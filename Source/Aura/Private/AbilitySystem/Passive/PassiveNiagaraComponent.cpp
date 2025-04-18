// copyright


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActive);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		//AddWeakLambda 这种绑定方式的主要好处是，当绑定的对象被销毁时，委托不会保持对象的引用，从而避免悬空指针问题和内存泄漏。
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* ASC)
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				AuraASC->ActivePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActive);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActive(const FGameplayTag& AbilityTag, bool bActivate)
{
	// 判断技能标签是否一致
	if(AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		// 判断是否需要激活
		if(bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
