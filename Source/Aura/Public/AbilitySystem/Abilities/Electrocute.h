// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	// 当前等级技能描述
	virtual FString GetDescription(int32 Level) override;
	// 下一等级技能描述
	virtual FString GetNextLevelDescription(int32 Level) override;
};
