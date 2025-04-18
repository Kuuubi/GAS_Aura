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
	* 被动技能标签
	*/
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	
	/*
	* 伤害类型标签
	*/
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	/*
	* 伤害类型对应属性抗性标签
	*/
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	/*
	* 伤害类型对应Debuff标签
	*/
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcan;
	FGameplayTag Debuff_Physical;

	// Debuff相关标签
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;
	FGameplayTag Debuff_Duration;
	
	/*
	* 伤害类型对应抗性
	*/
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	/*
	* 伤害类型对Debuff
	*/
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuff;

	/*
	* 能力标签
	*/
	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;

	//技能状态
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	//技能类型
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	//主动技能
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;

	// 被动技能
	FGameplayTag Abilities_Passive_HaloOfProtection; // 守护光环
	FGameplayTag Abilities_Passive_LifeSiphon; // 生命回复
	FGameplayTag Abilities_Passive_ManaSiphon; // 蓝量回复

	/*
	* 技能冷却标签
	*/
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;

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
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	// 阻止玩家控制器输入相关事件标签
	FGameplayTag Player_Block_InputPressed; // 阻止按键按下
	FGameplayTag Player_Block_InputHeld; // 阻止按键悬停
	FGameplayTag Player_Block_InputReleased; // 阻止键位抬起
	FGameplayTag Player_Block_CursorTrace; // 阻止鼠标追踪

private:
	//静态成员变量
	static FAuraGameplayTags GameplayTags;

};
