// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	// 技能类型标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	//技能标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	//冷却标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CooldownTag= FGameplayTag();

	//技能状态标签
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	//输入映射标签
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	//技能图标
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	//背景材质
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	//玩家等级要求
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	//当前技能使用的技能类GA
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	//根据标签查找技能相关数据
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
