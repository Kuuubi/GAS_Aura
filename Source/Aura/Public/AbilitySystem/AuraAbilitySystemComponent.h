// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//声明多播委托Tag
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/)
//拥有的技能
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
//更新技能状态
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
// 装备新技能
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PreviousSlot*/);
// 结束技能激活
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/)
// 被动技能激活/结束同时激活/结束被动奶瓜特效
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/)

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//AbilityActorInfo初始化完成后调用
	void AbilityActorInfoSet();

	//创建AssetTags委托
	FEffectAssetTags EffectAssetTags;
	//创建拥有的技能委托
	FAbilitiesGiven AbilitiesGivenDelegate;
	//创建更新技能状态委托
	FAbilityStatusChanged AbilityStatusChanged;
	// 创建更新装备的新技能标签
	FAbilityEquipped AbilityEquipped;
	// 结束技能激活的委托
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	// 激活/结束被动奶瓜特效
	FActivePassiveEffect ActivePassiveEffect;

	//赋予角色主动能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	//赋予角色被动能力
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	//角色能力是否添加完毕
	bool bStartupAbilitiesGiven = false;

	//按键按下时
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	//按键按住时
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	//按键释放时
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	//遍历可激活技能
	void ForEachAbility(const FForEachAbility& Delegate);
	
	//通过Spec获取Abilities标签
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//通过Spec获取Input标签
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//通过Spec获取技能状态标签
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	//通过AbilityTag获取技能状态标签
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	//通过AbilityTag获取装备技能的槽位
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);
	
	// 检查技能槽位是否为空
	bool SlotIsEmpty(const FGameplayTag& Slot);
	// 检查技能是否装备到指定槽位
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	// 检查技能是否已经有槽位
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	// 获取指定技能槽位装备的AbilitySpec
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	// 检查技能是否是被动技能
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;
	// 分配指定槽位给技能
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	/**
 	 * 多播被动特效委托广播，让每个客户端都可以看到特效
 	 * @param AbilityTag 被动技能标签
 	 * @param bActivate 激活或者关闭
 	 */
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	
	//通过AbilityTag获取指定Spec，否则返回空指针表示不能使用该技能
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	//要增加的属性
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	//服务器RPC增加属性
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	//满足技能的玩家等级要求更新技能状态
	void UpdateAbilityStatus(int32 Level);

	//服务器RPC消耗技能点
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	// 服务器RPC修改技能的输入标签实现装备
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Slot);

	// 客户端RPC将广播装备上的新技能的标签，状态，新插槽，旧插槽
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	//根据游戏标签获取技能描述文本
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// 根据Spec删除输入标签
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	// 清除装备技能栏插槽的技能
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	// 判断技能是否在插槽
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
	
protected:

	//广播拥有技能
	virtual void OnRep_ActivateAbilities() override;
	
	//客户端RPC效果应用回调函数
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	//客户端RPC广播更新技能状态
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
