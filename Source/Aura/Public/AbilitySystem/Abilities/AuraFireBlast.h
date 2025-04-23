// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/AuraFireBall.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	//当前等级技能描述
	virtual FString GetDescription(int32 Level) override;
	//下一等级技能描述
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 生成火球
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireballs();

protected:
	UPROPERTY(EditDefaultsOnly, Category= "FireBlast")
	int32 NumFireBalls = 12;

private:

	// 生成Actor类
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
