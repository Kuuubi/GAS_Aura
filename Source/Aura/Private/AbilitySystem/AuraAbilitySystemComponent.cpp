// copyright


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//效果应用委托到自己
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
	
}

//添加角色能力
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	//遍历StartupAbilities
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
	bStartupAbilitiesGiven = true;
	//广播拥有的技能
	AbilitiesGivenDelegate.Broadcast(this);
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

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	//作用域锁
	FScopedAbilityListLock ActiveScopedLock(*this);
	//遍历可激活能力列表
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//检查Delegate是否有绑定的函数，如果没有绑定返回false
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("在函数 %hs 未能执行委托"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		//通过Spec获取AbilityTags
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			//匹配父标签
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	//输入标签在标签容器中，不必从Ability获取
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		//广播拥有的技能
		AbilitiesGivenDelegate.Broadcast(this);
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
