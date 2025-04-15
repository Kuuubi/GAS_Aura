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
	
	//多枚火球术，跟踪目标
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomeingTarget);

protected:
	//火球发射角度
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float ProjectileSpread = 90.f;

	//火球生成最大数量
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	int32 MaxNumProjectiles = 5;

	//火球转向目标最小弧度
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float HomingAccelerationMin = 1600.f;

	//火球转向目标最大弧度
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float HomingAccelerationMax = 3200.f;

	//火球启用追踪
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	bool bLaunchHomingProjectiles = true;
};
