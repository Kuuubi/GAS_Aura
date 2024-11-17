// copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	//检查权威
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//判断实现战斗接口
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		//获取要发射火球的骨骼插槽位置
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		//火球生成的位置向量减去目标位置向量得到旋转就是要朝哪边飞的方向
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		//旋转俯仰角
		Rotation.Pitch = 0.f;

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

		//设置伤害
		//获取ASC
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		//SpecHandle
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		//设置生成的抛射物SpecHandle
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		//最终生成
		Projectile->FinishSpawning(SpawnTransform);
	}
}
