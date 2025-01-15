// copyright


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Aura/Public/AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);//获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetManaCost(Level);//获取冷却时间
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
			"<Damage> %d </>"
			"<Default>点火焰伤害,并有一定几率造成燃烧</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			Damage
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
			"<Damage> %d </>"
			"<Default>点火焰伤害,并有一定几率造成燃烧</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, NumProjectiles),
			Damage
			);
	}

}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);//获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetManaCost(Level);//获取冷却时间
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
		"<Damage> %d </>"
		"<Default>点火焰伤害,并有一定几率造成燃烧</>"),
		
		//数值
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level, NumProjectiles),
		Damage
		);
}