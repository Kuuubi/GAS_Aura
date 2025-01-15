// copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//设置输入标签
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	//获取当前技能等级描述
	virtual FString GetDescription(int32 Level);
	//获取下一技能等级描述
	virtual FString GetNextLevelDescription(int32 Level);
	//获取技能锁定状态描述
	static FString GetLockedDescription(int32 Level);

protected:
	
	//获取技能消耗蓝量
	float GetManaCost(float InLevel = 1.f) const;
	//获取技能冷却时间
	float GetCooldown(float InLevel = 1.f) const;
};
