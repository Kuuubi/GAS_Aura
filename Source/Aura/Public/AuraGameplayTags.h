// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * 单例模式，内部包含NativeGameplayTags
 */

//定义结构体
struct FAuraGameplayTags
{
public:
	//静态函数Get
	static const FAuraGameplayTags& Get() { return  GameplayTags;}
	//初始化Native游戏标签
	static  void InitializeNativeGameplayTags();

	/*
	 * 主要属性
	 */
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/*
	* 次要属性
	*/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/*
	* 属性抗性标签
	*/
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	/*
	* 元属性标签
	*/
	FGameplayTag Attributes_Meta_IncomingXP;

	/*
	* 输入标签
	*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
	/*
	* 伤害类型标签
	*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	/*
	* 伤害类型对应抗性
	*/
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	/*
	* 能力标签
	*/
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	//技能
	FGameplayTag Abilities_Fire_FireBolt;

	/*
	* 冷却标签
	*/
	FGameplayTag Cooldown_Fire_FireBolt;

	/*
	* 受击标签
	*/
	FGameplayTag Effects_HitReact;

	/*
	* 攻击方式标签
	*/
	//使用武器攻击部位标签
	FGameplayTag CombatSocket_Weapon;
	//左手攻击部位标签
	FGameplayTag CombatSocket_LeftHand;
	//右手攻击部位标签
	FGameplayTag CombatSocket_RightHand;
	//尾巴攻击部位标签
	FGameplayTag CombatSocket_Tail;

	//攻击动作索引标签
	FGameplayTag Montage_Attack_1;;
	FGameplayTag Montage_Attack_2;;
	FGameplayTag Montage_Attack_3;;
	FGameplayTag Montage_Attack_4;;

private:
	//静态成员变量
	static FAuraGameplayTags GameplayTags;

};
