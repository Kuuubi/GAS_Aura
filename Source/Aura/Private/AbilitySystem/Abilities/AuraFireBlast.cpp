// copyright


#include "AbilitySystem/Abilities/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	return FString::Printf(TEXT(
		//标题
		"<Title>火球爆炸术</>\n"
		
		//细节
		"<Small>等级：</><Level> %d </>\n"
		"<Small>冷却时间：</><Cooldown> %.1f </>\n"
		"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"
		//描述
		"<Default>向四周发射,"
		" %d 颗火球,碰到敌人造成</><Damage> %.1f </>"
		"<Default>点火焰伤害,并有一定几率造成燃烧,之后返回施法位置</>"),
		//数值
		Level,
		Cooldown,
		ManaCost,
		NumFireBalls,
		ScaledDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	return FString::Printf(TEXT(
		//标题
		"<Title>下一级</>\n"
		
		//细节
		"<Small>等级：</><Level> %d </>\n"
		"<Small>冷却时间：</><Cooldown> %.1f </>\n"
		"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"
		//描述
		"<Default>向四周发射,"
		" %d 颗火球,碰到敌人造成</><Damage> %.1f </>"
		"<Default>点火焰伤害,并有一定几率造成燃烧,之后返回施法位置</>"),
		//数值
		Level,
		Cooldown,
		ManaCost,
		NumFireBalls,
		ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireballs()
{
	// 获取到角色朝向和位置
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	// 通过函数均匀计算每个生成的火球需要的旋转
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360, NumFireBalls);

	TArray<AAuraFireBall*> FireBalls; // 生成所需的火球数组
	for (const FRotator& Rotator : Rotators)
	{
		// 创建变换
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion()); // 四元数

		// 创建火球 使用 SpawnActorDeferred 来生成对象时，UE 会延迟实际的对象生成过程，这样就有机会在完全初始化对象之前进行自定义配置。
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// 设置火球的伤害配置
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		// 设置返回的目标
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults(); // 对伤害配置进行修改
		FireBall->SetOwner(GetAvatarActorFromActorInfo()); // 设置拥有者

		FireBalls.Add(FireBall);

		//在配置完火球后，调用FinishSpawning将火球正式添加到场景中
		FireBall->FinishSpawning(SpawnTransform);

	}
	
	return FireBalls;
}
