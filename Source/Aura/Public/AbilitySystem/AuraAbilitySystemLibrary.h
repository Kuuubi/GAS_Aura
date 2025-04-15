// copyright

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	//构造控件控制层
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);

	//获取Overlay控件控制层
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	//获取AttributeMenu控件控制层
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	//获取SpellMenu控件控制层
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	//根据敌人职业和等级初始化属性
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	//赋予初始GA
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	//获取角色类信息数据资产
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	//获取技能信息数据资产
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static UAbilityInfo* GetAbilityClassInfo(const UObject* WorldContextObject);

	// =================================================================================================================
	// Getter
	
	//判断是否成功格挡
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	//判断是否成功暴击
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// 判断Debuff是否生效
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取Debuff的伤害
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取Debuff的持续时间
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取Debuff的频率
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取伤害类型游戏标签
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取死亡冲击击飞方向
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	// 获取攻击击退方向
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	// Setter
	
	//设置是否成功格挡
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	//设置是否成功暴击
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	//设置Debuff是否生效
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	//设置Debuff的伤害
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	//设置Debuff的持续时间
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	//设置Debuff的频率
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	//设置伤害类型游戏标签
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	// 设置死亡冲击击飞方向
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	// 设置攻击击退方向
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	// =================================================================================================================

	//获取一定半径范围内所有在线的玩家
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static void GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	/**
	 * 获取距离原点位置最近的几个目标
	 * @param MaxTargets 最大最近目标的数量
	 * @param Actors 需要计算的目标数组
	 * @param OutClosestTargets 返回最近的目标数组
	 * @param Origin 原点位置
	 */
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	//判断不是队友
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	// 应用Debuff GE
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | DamageEffect ")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	/**
 	* 这个函数根据传入的值计算均匀分布的多段角度，
 	*
 	* @param Forward 正前方向
 	* @param Axis 基于旋转的轴
 	* @param Spread 角度范围
 	* @param NumRotators 分段数
 	*
 	* @return TArray<FRotator&> 返回每段角度的中间角度的数组
 	*
 	* @note 这个函数用于在技能生成投掷物的函数逻辑中
 	*/
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	/**
 	* 这个函数根据传入的值计算均匀分布的多段朝向
 	*
 	* @param Forward 正前方向
 	* @param Axis 基于旋转的轴
 	* @param Spread 角度范围
 	* @param NumVectors 分段数
 	*
 	* @return TArray<FVector&> 返回每段角度的中间角度的朝向数组
 	*
 	* @note 这个函数用于在技能生成投掷物的函数逻辑中
 	*/
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);

	//通过敌人等级获取经验奖励值
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
