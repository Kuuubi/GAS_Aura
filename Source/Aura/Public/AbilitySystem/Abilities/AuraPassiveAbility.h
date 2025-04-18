// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	/**
 	 * 覆写激活技能
 	 * @param Handle 技能实例的句柄
 	 * @param ActorInfo 技能拥有者信息
 	 * @param ActivationInfo 激活信息
 	 * @param TriggerEventData 游戏事件数据
 	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/**
     * 结束技能回调
     * @param AbilityTag 要结束的技能标签
     */
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
