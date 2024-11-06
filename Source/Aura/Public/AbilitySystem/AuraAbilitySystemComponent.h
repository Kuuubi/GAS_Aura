// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//声明多播委托Tag
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags*/)


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

	//公开广播Tag
	FEffectAssetTags EffectAssetTags;

	//添加角色能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	//按键释放时
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	//按键按下时
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	//效果应用回调函数
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	
};
