// copyright


#include "AbilitySystem/Abilities/AranceShards.h"

FString UAranceShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	if (Level == 1)
	{	
		return FString::Printf(TEXT(
			//标题
			"<Title>奥术碎片</>\n"
			
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"

			//描述
			"<Default>魔法光圈位置生成 %d 块奥术碎片，攻击附近敌人，"
			"并造成</> <Damage>%d</> <Default>点奥术伤害。</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage
			);
	}
	else
	{	
		return FString::Printf(TEXT(
			//标题
			"<Title>奥术碎片</>\n"
			
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"

			//描述
			"<Default>魔法光圈位置生成 %d 块奥术碎片，攻击附近敌人，"
			"并造成</> <Damage>%d</> <Default>点奥术伤害。</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, MaxNumShards - 1),
			ScaledDamage
			);
	}

}

FString UAranceShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
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
		"<Default>魔法光圈位置生成 %d 块奥术碎片，攻击附近敌人，"
		"并造成</> <Damage>%d</> <Default>点奥术伤害。</>"),

		//数值
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level, MaxNumShards - 1),
		ScaledDamage
		);
}
