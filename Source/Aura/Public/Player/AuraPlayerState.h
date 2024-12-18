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
	//重载复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//重载GetASC
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
	
	//获取玩家经验值
	FORCEINLINE int32 GetPlayerXP() const { return XP; };
	//获取玩家等级
	FORCEINLINE int32 GetPlayerLevel() const { return Level; };

	//增加经验值
	void AddToXP(int32 InXP);
	//增加等级
	void AddToLevel(int32 InLevel);
	//增加可用属性点
	//增加可用技能点
	
	//设置经验值
	void SetXP(int32 InXP);
	//设置等级
	void SetLevel(int32 InLevel);
	//设置可用属性点
	//设置可用技能点
	
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
	

	//服务器上经验值变化时回调
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	//服务器上等级变化时回调
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
