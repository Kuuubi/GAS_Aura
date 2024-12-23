// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//声明多播委托Tag
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags*/)
//拥有的技能
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);


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
	///创建拥有的技能委托
	FAbilitiesGiven AbilitiesGivenDelegate;

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

	//要增加的属性
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	//服务器RPC增加属性
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	//客户端广播拥有技能
	virtual void OnRep_ActivateAbilities() override;
	
	//效果应用回调函数
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	
};
