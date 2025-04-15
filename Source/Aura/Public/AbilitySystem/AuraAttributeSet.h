// copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//结构宏
USTRUCT()
//属性影响结构体
struct FEffectProperties
{
	//正文宏
	GENERATED_BODY()

	FEffectProperties(){}

	//EffectContextHandle
	FGameplayEffectContextHandle EffectContextHandle;

	//指针，确保初始化null
	//Source
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	//Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

//typedef是专一FGameplayAttribute()的签名，但是 TStaticFuncPtr 对任何签名都有用。
//静态委托实例
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
//模板
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	//声明属性函数
	UAuraAttributeSet();
	//注册变量，指定哪些属性需要在服务器和客户端同步，并定义每个属性的复制条件
	virtual  void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//属性值变化前触发函数
	virtual  void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//效果应用后触发函数
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	//属性值变化后触发函数
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	//Map标签到属性
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
	 * 主要属性
	 */
	
	//力量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength,  Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	//智慧
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence,  Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	//韧性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience,  Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	//活力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor,  Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	
	/*	
     * 次要属性
    */

	//护甲
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor,  Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	//护甲穿透
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration,  Category = "Secondary Attributes")
    FGameplayAttributeData ArmorPenetration;
    ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	//格挡几率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance,  Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	//暴击几率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance,  Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	//暴击伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage,  Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	//暴击抵抗
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance,  Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	//生命值恢复速度
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration,  Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	//法力值恢复速度
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration,  Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,  Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana,  Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	
	/*
	* 属性抗性
	*/
	
	//火属性抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance,  Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);
	
	//雷属性抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance,  Category = "Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);
	
	//魔法抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance,  Category = "Resistance Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);
	
	//物理抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance,  Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	
	/*
	 * 重要属性
 	*/
	
	//生命值
	//BlueprintReadOnly设置蓝图只读
	//ReplicatedUsing为当服务端修改Health数值时的回调，拥有Replicated属性时，其值会从服务器自动复制到客户端
	//Category设置类别
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,  Category = "Vital Attributes")
	FGameplayAttributeData Health;
	//生命值属性访问器
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	

	//法力值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana,  Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	
	/*
	 * 元属性
	 */
	//传入伤害元属性
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	//传入XP元属性
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);
	

//GE预测旧值
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OnRep_FireResistance) const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OnRep_LightningResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OnRep_ArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OnRep_PhysicalResistance) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	// 处理传入伤害时处理的逻辑
	void HandleIncomingDamage(const FEffectProperties& Props);
	
	// 处理传入经验时处理的逻辑
	void HandleIncomingXP(const FEffectProperties& Props);
	
	// 处理传入Debuff时处理的逻辑
	void Debuff(const FEffectProperties& Props);

	//设置属性影响结构体函数
	//获取效果的来源和目标
	void SetFEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	
	//显示浮动文本
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	
	//发送经验事件
	void SendXPEvent(const FEffectProperties& Props);

	//判断升级满血
	bool bTopOffHealth = false;
	//判断升级满蓝
	bool bTopOffMana = false;
};
