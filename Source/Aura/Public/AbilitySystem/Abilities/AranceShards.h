// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AranceShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAranceShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 当前等级技能描述
	virtual FString GetDescription(int32 Level) override;
	// 下一等级技能描述
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
