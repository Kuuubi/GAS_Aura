//预处理指令 确保这个头文件只被包含（include）一次，防止重复定义
#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

// 伤害效果参数结构体
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	// ctx
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	// 伤害GE
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	// 来源ASC
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	// 目标ASC
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	// 基础伤害
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	// 技能等级
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	// 技能伤害类型标签
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	// 造成Debuff的几率
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
	// Debuff每次造成的伤害
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
	// Debuff频率（每秒施加一次伤害）
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	// Debuff持续时间
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	// 死亡击飞冲击幅度
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;
	// 死亡时击飞冲击方向
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	// 攻击击退幅度
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;
	// 攻击击退概率
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;
	// 攻击击退方向
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	// 是否为范围伤害
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	// 内半径伤害距离：在此半径内的所有目标都将受到完整的伤害
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	// 外半径伤害距离：距离伤害衰减，超出此距离外的目标将无法受到伤害
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	// 伤害的中心点
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
};

//在蓝图中可作为类型使用
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	// Getter
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool GetIsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	// Setter
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetIsRadialDamage(const bool& InIsRadialDamage) { bIsRadialDamage = InIsRadialDamage; }
	void SetRadialDamageInnerRadius(const float& InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(const float& InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	//返回用于序列化的静态结构体
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** 创建一个副本，用于后续网络复制或者后续修改 */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this; //WithCopy 设置为true，就可以通过赋值操作进行拷贝
		if (GetHitResult())
		{
			// 深拷贝 hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	//网络序列化
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
protected:

	//是否格挡
	UPROPERTY()
	bool bIsBlockedHit = false;

	//是否暴击
	UPROPERTY()
	bool bIsCriticalHit = false;

	// Debuff是否生效
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	// Debuff伤害
	UPROPERTY()
	float DebuffDamage = 0.f;
	// Debuff持续时间
	UPROPERTY()
	float DebuffDuration = 0.f;
	// Debuff频率
	UPROPERTY()
	float DebuffFrequency = 0.f;

	// 伤害类型游戏标签
	TSharedPtr<FGameplayTag> DamageType;

	// 死亡时击飞冲击方向
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	// 攻击击退方向
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	// 是否为范围伤害
	UPROPERTY()
	bool bIsRadialDamage = false;

	// 内半径伤害距离：在此半径内的所有目标都将受到完整的伤害
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	// 外半径伤害距离：距离伤害衰减，超出此距离外的目标将无法受到伤害
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	// 伤害的中心点
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
