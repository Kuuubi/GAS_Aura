// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*); // Actor初始化ASC完成后委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor); // Actor死亡后的委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/)

//攻击方式结构体
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	//攻击蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	//攻击动作标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	//攻击部位标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;;

	//击中音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;

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



/**
 * 
 */
class AURA_API ICombatInterface	
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//获取等级
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();
	
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
	virtual void Die(const FVector& DeathImpulse) = 0;

	//判断角色死亡
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead();

	//获取Avatar
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	//获取攻击方式蒙太奇数组
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	//获取溅血奶瓜特效
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	//根据攻击动作标签获取攻击方式结构体
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	//获取召唤物数量计数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	//召唤物数量计数增加
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncremenetMinionCount(int32 Amount);

	//获取敌人职业类型
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	// 获取ASC注册成功后的委托
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	// 获取Actor死亡后的委托
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	/**
 	 * 获取角色受到伤害触发的委托
 	 * @return 委托
 	 */
	virtual FOnDamageSignature& GetOnDamageSignature() = 0;

	//设置为处于持续释放技能
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);

	// 获取武器骨骼网格体
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	/**
	 * 获取角色是否被持续攻击状态
	 * @return 布尔值，如果处于返回true
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked();

	/**
	 * 设置角色是否被持续攻击状态
	 * @param bInShock 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);
};
