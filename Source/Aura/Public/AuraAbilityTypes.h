//预处理指令 确保这个头文件只被包含（include）一次，防止重复定义
#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

//在蓝图中可作为类型使用
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	//获取
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	//设置
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	
	//返回用于序列化的静态结构体
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
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
