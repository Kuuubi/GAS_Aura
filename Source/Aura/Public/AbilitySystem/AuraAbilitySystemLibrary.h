// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
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

	//获取Overlay组件控制器
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	//获取AttributeMenu组件控制器
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	//根据敌人职业和等级初始化属性
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	//赋予初始GA
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	//获取角色类信息数据资产
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | CharacterClassDefaults ")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	//判断是否成功格挡
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	//判断是否成功暴击
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	//设置是否成功格挡
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	//设置是否成功暴击
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayEffects ")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	//获取一定半径范围内所有在线的玩家
	UFUNCTION(BlueprintCallable,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static void GetLivePlayerWithnRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	//判断不是队友
	UFUNCTION(BlueprintPure,  Category="AuraAbilitySystemLibrary | GameplayMechanics ")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
};
