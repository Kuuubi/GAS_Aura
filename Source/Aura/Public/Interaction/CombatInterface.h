// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

//攻击蒙太奇，攻击方式标签，骨骼插槽结构体
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	//攻击蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	//攻击方式标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	//攻击时触发伤害的骨骼插槽
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName CombatTipSocketName; 
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;

/**
 * 
 */
class AURA_API ICombatInterface	
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//获取等级
	virtual int32 GetPlayerLevel();
	
	//获取攻击骨骼插槽位置
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	//朝目标位置转向
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	//获取受击反应蒙太奇动画
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	//角色死亡
	virtual void Die() = 0;

	//判断角色死亡
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead();

	//获取Avatar
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	//获取攻击方式蒙太奇数组
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
};
