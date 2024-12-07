// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	//获取生成位置
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	//获取召唤随机的召唤物类型
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	//召唤的数量
	UPROPERTY(EditDefaultsOnly, Category= "Summoning")
	int32 NumMinions = 5;

	//召唤生成的角色类
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	//召唤位置距离召唤师最近的距离
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;
	
	//召唤位置距离召唤师最远的距离
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 300.f;
	
	//召唤位置在召唤师前面的角度范围
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
	
	
	
};
