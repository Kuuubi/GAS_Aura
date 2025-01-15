// copyright


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//获取属性集
	check(AttributeInfo)
	//循环Map
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		//属性发生变化时广播
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	
	//玩家可用属性点变化广播
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	//获取属性集
	check(AttributeInfo)

	//查找标签
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	//广播初始值
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());

}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute) const
{
	//属性信息
	FAuraAttributeInfo Info =  AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	//设置其属性值
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	//广播属性信息
	AttributeInfoDelegate.Broadcast(Info);
}
