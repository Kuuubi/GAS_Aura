// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//声明多播委托Tag
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags*/)
//拥有的技能
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
//更新技能状态
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* AbilityTag */, const FGameplayTag& /* StatusTag */, int32 /* AbilityLevel */);
// 装备新技能
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /* AbilityTag */, const FGameplayTag& /* Status */, const FGameplayTag& /* Slot */, const FGameplayTag& /* PreviousSlot */);


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

	//赋予角色主动能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	//赋予角色被动能力
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	//角色能力是否添加完毕
	bool bStartupAbilitiesGiven = false;

	//按键释放时
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	//按键按下时
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
	//通过AbilityTag获取技能输入标签
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
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
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	//根据游戏标签获取技能描述文本
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// 根据Spec删除输入标签
	void ClearSlot(FGameplayAbilitySpec* Spec);
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
