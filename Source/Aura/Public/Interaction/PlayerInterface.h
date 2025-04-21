// copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//通过XP查找当前等级
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	//获取当前经验值
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerXP() const;

	//获取要奖励多少可用属性点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	//获取要奖励多少可用技能点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	//获取可用属性点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	//获取可用属性点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	//增加经验值
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	//增加玩家等级
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	//增加可用属性点
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	//增加可用技能点
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPointsReward);

	//升级
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	// 显示奥术攻击范围
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	// 隐藏奥术攻击范围
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
};
