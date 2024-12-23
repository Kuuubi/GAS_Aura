// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	//复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//GetASC
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//GetAS
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	//升级信息数据
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	//经验值变化委托
	FOnPlayerStatChanged OnXPChangedDelegate;
	//等级变化委托
	FOnPlayerStatChanged OnLevelChangedDelegate;
	//可用属性点变化委托
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	//可用技能点变化委托
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;
	
	//获取玩家经验值
	FORCEINLINE int32 GetPlayerXP() const { return XP; };
	//获取玩家等级
	FORCEINLINE int32 GetPlayerLevel() const { return Level; };
	//获取玩家可用属性点
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; };
	//获取玩家可用技能点
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; };

	//增加经验值
	void AddToXP(int32 InXP);
	//增加等级
	void AddToLevel(int32 InLevel);
	//增加可用属性点
	void AddToAttributePoints(int32 InPoints);
	//增加可用技能点
	void AddToSpellPoints(int32 InPoints);
	
	//设置经验值
	void SetXP(int32 InXP);
	//设置等级
	void SetLevel(int32 InLevel);
	//设置可用属性点
	void SetAttributePoints(int32 InPoints);
	//设置可用技能点
	void SetSpellPoints(int32 InPoints);
	
protected:
	UPROPERTY(VisibleAnywhere)
	//技能系统组件
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	//属性集
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	//玩家经验值
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP);
	int32 XP = 1;

	//玩家等级
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level);
	int32 Level = 1;

	//玩家可用属性点
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints);
	int32 AttributePoints = 0;

	//玩家可用技能点
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints);
	int32 SpellPoints = 0;

	//服务器上经验值变化时回调
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	//服务器上等级变化时回调
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	//服务器上可用属性点变化时回调
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	//服务器上可用技能点变化时回调
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
