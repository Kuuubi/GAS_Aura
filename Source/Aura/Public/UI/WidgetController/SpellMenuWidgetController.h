// copyright

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

//菜单消耗，装备按钮启用布尔值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
//装备按钮广播技能类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
// 装备技能后取消选中技能
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityType);

//本地选中的技能和该技能状态
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	//广播初始值
	virtual void BroadcastInitialValues() override;
	//属性值变化时委托广播
	virtual  void BindCallbacksToDependencies() override;

	//创建技能点委托
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;

	//菜单消耗，装备按钮启用布尔值
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	// 醒目装备按钮广播技能类型
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	// 取消醒目装备按钮广播技能类型
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;

	// 装备技能后取消选中技能
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;

	//根据选中技能的Tag获取技能状态控制菜单按钮启用
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	//按下消耗技能点
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	// 再次点击相同的取消选中按钮
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	// 按下装备按钮
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	// 选中技能球的输入标签和类型标签
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	// 装备新技能
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	
	//菜单消耗，装备按钮启用
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);

	//本地技能和状态
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	//本地技能点数
	int32 CurrentSpellPoints = 0;
	
	// 等待技能装备布尔值
	bool bWaitingForEquipSelection = false;

	// 存储选中的技能插槽的输入标签
	FGameplayTag SelectedSlot;
};
