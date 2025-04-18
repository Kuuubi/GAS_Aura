// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * 保存鼠标命中的信息
	 * @param HitResult 在技能中通过TargetDataUnderMouse的task获取到的结果
	 */
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	/**
	 * 保存拥有当前技能的玩家控制器
	 */
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	/**
 	 * 闪电链命中的第一个目标
 	 * @param BeamTargetLocation 鼠标点击的位置
 	 * @note 有可能鼠标点击位置和角色中间会被其他敌人阻挡，将被阻挡位置设置为第一个目标
	 */
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	/**
	 * 获取命中目标一定范围内最近的目标敌人
	 * @param OutAdditionalTargets 返回获取到的最近的目标数组
	 */
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	/**
	 * 鼠标命中的主要目标敌人死亡
	 * @param DeadActor 主要敌人
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	/**
	 * 目标敌人附近的额外敌人死亡
	 * @param DeadActor 额外敌人 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

protected:

	// 鼠标命中的位置
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;

	// 鼠标命中的Actor
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;

	// 拥有当前技能的玩家控制器
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	// 拥有当前技能的玩家角色
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	// 最大连接附近敌人的闪电链数
	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxNumShockTargets = 5;
};
