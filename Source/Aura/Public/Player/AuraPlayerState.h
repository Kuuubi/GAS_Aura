// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	//网络同步复制
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//GetASC
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//GetAS
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	//获取玩家等级
	FORCEINLINE int32 GetPlayerLevel() const { return Level; };
	
protected:
	UPROPERTY(VisibleAnywhere)
	//技能系统组件
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	//属性集
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	
	//玩家等级
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level);
	int32 Level = 1;

	//GE预测旧值
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
