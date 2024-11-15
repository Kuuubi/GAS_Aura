// copyright


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//效果应用委托到自己
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
	
}

//添加角色能力
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	//循环为每个StartupAbilities调用AbilityClass
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		
		//创建能力Spec
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		//转换类型访问StartupInputTag
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//Spec动态添加标签
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			//赋予能力，可以接受const AbilitySpec
			GiveAbility(AbilitySpec);
			//另个是赋予能力并立即激活一次，可选 FGameplayEventData 参数，不接受const AbilitySpec
			// GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

//按键按下
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	//标签有效性
	if (!InputTag.IsValid()) return;
	//获取所有可激活能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//匹配标签
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//告知能力正在被按下
			AbilitySpecInputPressed(AbilitySpec);
			//激活能力
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

//按键释放
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	//标签有效性
	if (!InputTag.IsValid()) return;
	//获取所有可激活能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//匹配标签
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

//效果应用回调函数
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//游戏标签容器
	FGameplayTagContainer TagContainer;
	//获取游戏效果的资产标签
	EffectSpec.GetAllAssetTags(TagContainer);

	//广播标签容器
	EffectAssetTags.Broadcast(TagContainer);
}
