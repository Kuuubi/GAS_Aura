// copyright


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s: </> \n<Default>当前技能等级: </><Level>%d</>"), L"默认技能名称 - 技能名称 技能名称 技能名称 技能名称 技能名称", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>下一技能等级: </><Level>%d</> \n<Default>技能描述 技能描述 技能描述 技能描述 技能描述</>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>该技能角色等级要求: </><Level>%d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	//消耗蓝量
	float ManaCost = 0.f;
	//获取成本GE
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		//遍历GE影响的属性
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			//找到修改蓝量属性
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				//通过修饰符获取ScalableFloat，根据传入等级返回消耗蓝量
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	//冷却时间
	float Cooldown = 0.f;
	//获取冷却GE
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		//冷却是持续时间幅度
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
