// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	//造成伤害
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	
	//设置伤害GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//伤害曲线表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	//从攻击方式数组随机获取索引
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray (const TArray<FTaggedMontage>& TaggedMontage) const;

	//根据技能的伤害类型标签和技能等级获取伤害值
	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	
};
