	// copyright


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	//GESpecHandle
	 FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	//循环曲线表，通过SetByCaller设置属性伤害值
	// for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	// {
	// 	//根据等级获取技能伤害
	// 	const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
	// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	// }
	//根据等级获取技能伤害
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	
	//对目标应用GE
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); // 蓝图库里已经判断是否为空
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DebuffDuration = DebuffDuration;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	if (IsValid(TargetActor)) // 如果目标有效
	{
		// 获取到攻击对象和目标的朝向，并转换成角度
		// const FVector ToTaarget = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal(); // 归一化向量
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f; // 设置击退角度垂直向上45度
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
	}
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontage) const
{
	if (TaggedMontage.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontage.Num() - 1);
		return TaggedMontage[Selection];
	}
	return FTaggedMontage();
}

// float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
// {
// 	//获取技能伤害值
// 	checkf(DamageTypes.Contains(DamageType), TEXT("技能 [%s] 不包含 [%s] 伤害类型"), *GetNameSafe(this), *DamageType.ToString());
// 	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
// }
