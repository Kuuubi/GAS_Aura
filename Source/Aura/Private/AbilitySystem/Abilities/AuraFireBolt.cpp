// copyright


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Aura/Public/AuraGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	// const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);//获取技能伤害
	const float ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	if (Level == 1)
	{	
		return FString::Printf(TEXT(
			//标题
			"<Title>火球术</>\n"
			
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"//四舍五入1.0f

			//描述
			"<Default>发射 1 颗火球,敌人碰到时产生爆炸,并造成</>"
			//伤害
			"<Damage> %.1f </>"
			"<Default>点火焰伤害,并有一定几率造成燃烧</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			//Damage
			ScaledDamage
			);
	}
	else
	{
		return FString::Printf(TEXT(
			//标题
			"<Title>火球术</>\n"
	
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"//四舍五入1.0f

			//描述
			"<Default>发射 %d 颗火球,敌人碰到时产生爆炸,并造成</>"
			//伤害
			"<Damage> %.1f </>"
			"<Default>点火焰伤害,并有一定几率造成燃烧</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, NumProjectiles),
			//Damage
			ScaledDamage
			);
	}

}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	// const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);//获取技能伤害
	const float ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	return FString::Printf(TEXT(
		//标题
		"<Title>下一级</>\n"

		//细节
		"<Small>等级：</><Level> %d </>\n"
		"<Small>冷却时间：</><Cooldown> %.1f </>\n"
		"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"//四舍五入1.0f
		
		//描述
		"<Default>发射 %d 颗火球,敌人碰到时产生爆炸,并造成</>"
		//伤害
		"<Damage> %.1f </>"
		"<Default>点火焰伤害,并有一定几率造成燃烧</>"),
		
		//数值
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level, NumProjectiles),
		//Damage
		ScaledDamage
		);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomeingTarget)
{
	//检查权威
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	//判断实现战斗接口
	// ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	// if (CombatInterface)
	// {
	
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

	const FVector Forward = Rotation.Vector();

	//根据技能等级生成火球数量
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());

	//分段发射方向
	//TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlyRotatedVectors(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	//分段发射角度
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const FRotator& Rot : Rotations)
	{
		/* DrawDebugArrow
		UKismetSystemLibrary::DrawDebugArrow(
		   GetAvatarActorFromActorInfo(),
		   SocketLocation,
		   SocketLocation + Rot.Vector() * 75.f,
		   2,
		   FLinearColor::Green,
		   120,
		   1);
		   */
		
		//设置生成火球的位置
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//设置火球旋转为发射角度
		SpawnTransform.SetRotation(Rot.Quaternion());

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

		// 锁定敌人跟踪目标
		if (HomeingTarget && HomeingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomeingTarget->GetRootComponent();
		}
		else
		{
			//创建一个新的场景组件将其设置为目标
			// Projectile->ProjectileMovement->HomingTargetComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			//虽然目标组件是弱指针，但是导弹销毁或目标组件超出范围，这个创建的新对象将不会被清理，所以创建一个公共的
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			// 设置追踪点的世界位置
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		//火球以多大弧度转向目标
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		//抛射物移动组件启用追踪
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		//最终生成火球
		Projectile->FinishSpawning(SpawnTransform);
	}
}
