// copyright


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);// 获取技能伤害
	const float ManaCost = FMath::Abs(GetManaCost(Level));//获取消耗蓝量
	const float Cooldown = GetCooldown(Level);//获取冷却时间
	if (Level == 1)
	{	
		return FString::Printf(TEXT(
			//标题
			"<Title>闪电链</>\n"
			
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"

			//描述
			"<Default>使用闪电攻击敌人，命中敌人后，向外扩散 %d 条闪电链，"
			"并造成</> <Damage>%d</> <Default>点闪电伤害，"
			"在技能结束时对敌人造成</> <Time>%i</> <Default>秒的眩晕效果。</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, MaxNumShockTargets),
			ScaledDamage,
			static_cast<int>(DebuffDuration));
	}
	else
	{	
		return FString::Printf(TEXT(
			//标题
			"<Title>闪电链</>\n"
			
			//细节
			"<Small>等级：</><Level> %d </>\n"
			"<Small>冷却时间：</><Cooldown> %.1f </>\n"
			"<Small>消耗蓝量：</><ManaCost> %.1f </>\n\n"

			//描述
			"<Default>使用闪电攻击敌人，命中敌人后，向外扩散 %d 条闪电链，"
			"并造成</> <Damage>%d</> <Default>点雷电伤害，"
			"在技能结束时对敌人造成</> <Time>%i</> <Default>秒的眩晕效果。</>"),

			//数值
			Level,
			Cooldown,
			ManaCost,
			FMath::Min(Level, MaxNumShockTargets - 1),
			ScaledDamage,
			static_cast<int>(DebuffDuration));
	}

}

FString UElectrocute::GetNextLevelDescription(int32 Level)
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
		"<Default>使用闪电攻击敌人，命中敌人后，向外扩散 %d 条闪电链，"
		"造成</> <Damage>%d</> <Default>点雷电伤害，"
		"在技能结束时对敌人造成</> <Time>%i</> <Default>秒的眩晕效果。</>"),

		//数值
		Level,
		Cooldown,
		ManaCost,
		FMath::Min(Level, MaxNumShockTargets - 1),
		ScaledDamage,
		static_cast<int>(DebuffDuration));
}
