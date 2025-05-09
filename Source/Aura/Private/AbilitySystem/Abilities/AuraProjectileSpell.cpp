// copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	//检查权威
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//判断实现战斗接口
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//获取要发射火球的骨骼插槽位置
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
			GetAvatarActorFromActorInfo(),
			SocketTag
			);
		
		//火球生成的位置向量减去目标位置向量得到旋转就是要朝哪边飞的方向
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		//旋转俯仰角
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		
		//设置生成火球的位置
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//设置火球旋转
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//生成火球
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			//指定要生成的Actor类
			ProjectileClass,
			//生成的Actor的位置
			SpawnTransform,
			//拥有所生成的Actor的AActor
			GetOwningActorFromActorInfo(),
			//导致生成的Actor造成伤害的来源者
			Cast<APawn>(GetOwningActorFromActorInfo()),
			//控制与其他对象碰撞的情况下如何处理生成此Actor
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		//最终生成
		Projectile->FinishSpawning(SpawnTransform);

		/*
		 * 不再手动设置生成的抛射物SpecHandle，改用DamageEffectParams
		 *	===================================================================================================
		//设置伤害
		//获取ASC
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		//EffectContextHandle
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		//设置能力
		EffectContextHandle.SetAbility(this);
		//添加SourceObject
		EffectContextHandle.AddSourceObject(Projectile);
		//添加Actors数组
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		//添加命中结果
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);
		

		//SpecHandle
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		//获取标签单例
		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

		//遍历DamageTypes
		// for (auto& Pair : DamageTypes)
		// {
		// 	//曲线表根据等级获取技能伤害
		// 	const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		// 	//由调用者设置
		// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		// }

		// 获取技能伤害
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		// 由调用者设置
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);
		
		//设置生成的抛射物SpecHandle
		Projectile->DamageEffectSpecHandle = SpecHandle;
		*	===================================================================================================
		*/

		
	}
}
