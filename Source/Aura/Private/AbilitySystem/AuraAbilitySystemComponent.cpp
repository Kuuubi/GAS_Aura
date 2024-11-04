// copyright


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//效果应用委托到自己
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	
}

//效果应用回调函数
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//游戏标签容器
	FGameplayTagContainer TagContainer;
	//获取游戏效果的资产标签
	EffectSpec.GetAllAssetTags(TagContainer);

	//广播标签容器
	EffectAssetTags.Broadcast(TagContainer);
}
