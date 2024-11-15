// copyright


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UAuraProjectileSpell::SpawnProjectile()
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

		//设置生成火球的位置
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

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

		//生成
		Projectile->FinishSpawning(SpawnTransform);
	}
}
