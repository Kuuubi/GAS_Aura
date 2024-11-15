// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//生成抛射物
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();
	
	//蓝图类中设置要生成的火球类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
