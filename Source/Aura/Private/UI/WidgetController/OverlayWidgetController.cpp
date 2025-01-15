// copyright


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

//广播初始值函数
void UOverlayWidgetController::BroadcastInitialValues()
{
	//广播初始值
	
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());

	//广播玩家初始等级
	//OnPlayerLevelChangedDelegate.Broadcast(GetAuraPS()->GetPlayerLevel());

}

//绑定监听数值变化依赖
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//绑定玩家经验值变化回调
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	//绑定玩家等级变化回调
	GetAuraPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		});
	
	
	//生命值
	//调用属性值变化时调用
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		//指定的属性，绑定Lambda广播新值
		GetAuraAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

	//法力值
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});

	if (GetAuraASC())
	{
		// 装备新技能
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		//拥有技能委托
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			//调用广播技能信息数据
			BroadcastAbilityInfo();
		}
		else
		{
			//如果技能还未完成初始化，通过绑定委托，监听广播
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		//AssetTags委托
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{	
				//循环
				for(const FGameplayTag Tag : AssetTags)
				{
					//匹配标签
					//"Message.HealthPotion".匹配"Message"返回True，反之False
					FGameplayTag MessageTag =  FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						//查找数据表
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	//获取升级信息数据
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("无法找到 LevelUpInfo，请在PlayerState配置"));

	//获取当前等级
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	//获取当前最大等级
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	//计算百分比
	if (Level <= MaxLevel && Level > 0)
	{
		//获取当前等级升级要达到XP值 900XP
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		//获取上一级升级要达到XP值 300XP
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		//总进度 900XP - 300XP = 600XP
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		//当前进度 400XP - 300XP = 100XP 
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		//经验条进度百分比 100 / 600 = 0.16
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		//广播经验条进度百分比
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// 清除旧插槽
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// 如果前一个插槽是有效插槽，广播空信息。仅当装备已装备的技能时。
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 填充新插槽
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	
}
