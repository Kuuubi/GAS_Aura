// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	//当前等级技能描述
	virtual FString GetDescription(int32 Level) override;
	//下一等级技能描述
	virtual FString GetNextLevelDescription(int32 Level) override;
};
