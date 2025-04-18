// copyright


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))) // 获取ASC
	{
		// 绑定结束技能回调
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactivate);
	}
	
}

void UAuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	
	if (AbilityTags.HasTagExact(AbilityTag)) // 判断技能标签容器里是否包含此标签
	{
		// 结束技能
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
