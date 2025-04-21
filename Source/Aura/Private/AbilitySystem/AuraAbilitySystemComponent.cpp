// copyright


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//效果应用委托到自己
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
	
}

//赋予角色主动能力
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
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Unlocked);
			//赋予能力,不立即激活，可以接受const AbilitySpec
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	//广播拥有的技能
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	//遍历StartupAbilities
    	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
    	{
    		
    		//创建能力Spec
    		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

    		//赋予能力并立即激活一次，可选 FGameplayEventData 参数，不接受const AbilitySpec
    		GiveAbilityAndActivateOnce(AbilitySpec);
    	}
}

//按键按下时
void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	//标签有效性
	if (!InputTag.IsValid()) return;

	// 作用域锁
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	//获取所有可激活能力
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//匹配标签
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//告知能力正在被按下
			AbilitySpecInputPressed(AbilitySpec);
			//判断能力处于激活
			if (AbilitySpec.IsActive())
			{
				//将按下事件复制到服务器和所有相关的客户端
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

//按键按住时
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	//标签有效性
	if (!InputTag.IsValid()) return;

	// 作用域锁
	FScopedAbilityListLock ActiveScopedLock(*this);
	// 获取所有激活的能力
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

//按键释放时
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	//标签有效性
	if (!InputTag.IsValid()) return;

	// 作用域锁
	FScopedAbilityListLock ActiveScopedLock(*this);
	//遍历可激活能力列表
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//匹配标签
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
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

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	// 作用域锁
	FScopedAbilityListLock ActiveScopedLock(*this);
	// 遍历可激活能力列表
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.DynamicAbilityTags.HasTagExact(Slot);
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityClassInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive);
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivePassiveEffect.Broadcast(AbilityTag, bActivate);
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	//遍历可激活能力数组
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//获取能力标签
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			//标签匹配
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		//判断可用属性点够用
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			//通知服务器修改对应属性值
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	//创建Payload
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	//发送游戏事件
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	//消耗可用属性点
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	//获取技能信息数据资产
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityClassInfo(GetAvatarActor());
	//遍历结构体数组
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		//游戏标签无效
		if (!Info.AbilityTag.IsValid()) continue;
		//检查等级未达标
		if (Level < Info.LevelRequirement) continue;
		//检查技这个技能是不可激活的
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			//创建能力Spec
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			//Spec添加技能合格状态标签
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			//赋予技能
			GiveAbility(AbilitySpec);
			//标记为Dirty，使Spec复制到客户端
			MarkAbilitySpecDirty(AbilitySpec);
			//广播，更新技能状态
			ClientUpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	//获取技能Spec
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		//消耗技能点数
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);//获取技能状态
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))//合格状态
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Abilities_Status_Eligible);//删除合格状态标签
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Abilities_Status_Unlocked);//添加解锁状态标签
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))//装备或解锁状态
		{
			AbilitySpec->Level += 1;//升级
		}
		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);//客户端RPC广播更新技能状态
		MarkAbilitySpecDirty(*AbilitySpec);//立即复制到客户端
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Slot)
{
	// 比如技能从左键换成右键
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec); // 技能之前装备的槽位
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec); // 当前技能的状态标签

		// 查看选中的技能状态是否是已装备的或解锁的
		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status == GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// 处理被动技能的激活/结束
			if (!SlotIsEmpty(Slot)) // 技能槽位不为空
			{
				// 获取指定技能槽位装备的AbilitySpec
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if (SpecWithSlot)
				{
					// 技能槽位已装备相同的技能
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
						return;
					}
					
					// 判断是否装备的是被动技能
					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false); // 停用被动特效
						DeactivatePassiveAbility.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot)); // 结束技能
					}

					ClearSlot(SpecWithSlot);
				}
			}

			if (!AbilityHasAnySlot(*AbilitySpec)) // 技能没有装备到任何槽位
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle); // 激活
					MulticastActivatePassiveEffect(AbilityTag, true); // 激活被动特效
				}
			}

			// 分配指定槽位给技能
			AssignSlotToAbility(*AbilitySpec, Slot);
			
			MarkAbilitySpecDirty(*AbilitySpec); // 立即将其复制到每个客户端
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{

	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	//技能Spec有效
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			//获取当前技能等级描述，传入当前技能等级
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			//获取下一技能等级描述，传入下一技能等级
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	//如果技能Spec无效，就是锁定状态
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityClassInfo(GetAvatarActor());//获取技能信息数据资产
	
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTag(FAuraGameplayTags::Get().Abilities_None))
	{
		// 没选中任何技能时，没有描述
		OutDescription = FString();
	}
	else
	{
		// 获取技能锁定状态描述，传入玩家等级要求
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	
	OutNextLevelDescription = FString();//下一级技能等级描述为空
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	// 遍历可激活能力列表
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			// 删除输入标签
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		//广播拥有的技能
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
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
