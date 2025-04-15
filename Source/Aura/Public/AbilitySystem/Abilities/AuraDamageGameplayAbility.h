// copyright

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
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

	// 创建技能Debuff伤害效果参数结构体
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	// 获取技能的伤害数值
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

protected:
	
	//设置伤害GE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//伤害曲线表
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	// TMap<FGameplayTag, FScalableFloat> DamageTypes;
	// 伤害类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FGameplayTag DamageType;
	// 伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat Damage;

	// 造成Debuff的几率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debuff")
	float DebuffChance = 20.f;
	// Debuff每次造成的伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debuff")
	float DebuffDamage = 5.f;
	// Debuff频率（每秒施加一次伤害）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debuff")
	float DebuffFrequency = 1.f;
	// Debuff持续时间
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Debuff")
	float DebuffDuration = 5.f;

	// 死亡击飞冲击幅度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	float DeathImpulseMagnitude = 1000.f;

	// 攻击击退幅度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	float KnockbackForceMagnitude = 1000.f;

	// 攻击击退概率
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Knockback")
	float KnockbackChance = 0.f;

	//从攻击方式数组随机获取索引
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray (const TArray<FTaggedMontage>& TaggedMontage) const;

	//根据技能的伤害类型标签和技能等级获取伤害值
	//float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	
};
