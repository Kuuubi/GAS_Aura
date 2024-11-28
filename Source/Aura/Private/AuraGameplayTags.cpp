// copyright


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

//静态成员变量定义
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

//初始化Native游戏标签
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	//游戏标签管理器添加Native标签

	/* 主要属性 */
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("提高物理伤害")
		);
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("提高魔法伤害")
		);
		
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("增加护甲")
		);
		
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("提高生命值")
		);
	
	/* 次要属性 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("减少受到的伤害，提高格挡几率")
		);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("忽略护甲百分比值，增加暴击率")
		);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("受到攻击伤害减少几率")
		);
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("暴击率，造成双倍伤害")
		);
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("暴击伤害，暴击时造成额外伤害")
		);
		
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("暴击抵抗，降低敌人的暴击率")
		);
		
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("每秒生命值恢复速度")
		);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("每秒法力值恢复速度")
		);
	
	GameplayTags.Attributes_Secondary_MaxHealth= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("最大生命值")
		);
		
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("最大法力值")
		);

	/* 输入标签 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("鼠标左键")
		);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("鼠标右键")
		);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("键盘 1")
		);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("键盘 2")
		);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("键盘 3")
		);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("键盘 4")
		);

	/* 属性抗性标签 */
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Fire"),
	FString("火属性抗性")
	);

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Lightning"),
	FString("雷属性抗性")
	);
	
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Arcane"),
	FString("魔法抗性")
	);
	
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Resistance.Physical"),
	FString("物理抗性")
	);
	

	/* 伤害类型标签 */
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage"),
	FString("造成伤害")
	);
	
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Fire"),
	FString("造成火属性伤害")
	);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Lightning"),
	FString("造成雷属性伤害")
	);
	
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Arcane"),
	FString("造成魔法伤害")
	);
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Physical"),
	FString("造成物理伤害")
	);

	//伤害类型对应抗性
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	/* 受击标签 */
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Effects.HitReact"),
	FString("受到攻击")
	);
}
