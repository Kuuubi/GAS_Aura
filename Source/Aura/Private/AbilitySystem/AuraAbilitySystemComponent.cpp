// copyright


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//效果应用委托到自己
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

//效果应用回调函数
void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//游戏标签容器
	FGameplayTagContainer TagContainer;
	//获取游戏效果的资产标签
	EffectSpec.GetAllAssetTags(TagContainer);
	//循环
	for(const FGameplayTag Tag : TagContainer)
	{
		//TODO：将Tag广播到组件控制器
		//获取标签名称
		const FString Msg = FString::Printf(TEXT("GE Tag：%s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	}
}
