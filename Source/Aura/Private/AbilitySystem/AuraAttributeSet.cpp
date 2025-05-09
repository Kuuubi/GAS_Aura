// copyright


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilities/Public/GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	//Key
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	//添加Map标签到属性键值对
	/* 主要属性 */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);
	/* 次要属性 */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	
	/* 属性抗性 */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	
	/* 重要属性 */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	
}

//注册变量
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	 * 主要属性
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always)

	/*
	* 次要属性
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)

	/*
	* 属性抗性
	*/
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always)
	
	/*
	* 重要属性
	*/
	//设置Health属性在服务器上可以被Replicate，COND_复制条件有None，Always，Replicate
	//REPNOTIFY_默认值是OnChanged，也就是被更改时才会复制，Always始终即使不更改也会复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always)

	
}

//属性值变化前触发函数
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//变化前夹紧属性范围
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}


//属性影响结构体
void UAuraAttributeSet::SetFEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	//Source = 效果的来源，Target = 效果的目标
	
	//获取Source信息
	//EffectSpec
	Props.EffectContextHandle  = Data.EffectSpec.GetContext();
	
	//调用获取Original函数，获取来源的ASC
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	//访问控制器
	//判断有效
	//AbilityActorInfo是包装在T指针里的，先检查确保指针已存储后，包装器有效。则可以使用->运算符检查AvatarActor有效
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		//调用函数Get原始指针获取AvatarActor并存储
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		
		//现在可以获得例如，可以获取来源控制器，包装在弱指针里要Get
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		//但是如果ActorInfo是空指针，需要通过将其转换成Pawn来从AvatarActor本身获取
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			//换成Pawn
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				//源控制器=获取控制器
				Props.SourceController = Pawn->GetController();
			}
		}
		
		//如果源控制器不为空，可以得到Character的Pawn
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	
	//获取Target信息
	//检查目标AbilityActorInfo和AvatarActor有效性
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		//获取AvatarActor
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		//获取PlayerController
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		//AvatarActor转换成ACharacter
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		//获取Target的ASC
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


//属性值变化后触发函数
void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//创建结构体
	FEffectProperties Props;
	//将数据传给Props
	SetFEffectProperties(Data, Props);

	// 如果目标角色死亡直接返回
	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.SourceCharacter)) return;

	//变化后夹紧属性值
	//最大血量
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		//UE_LOG(LogTemp, Warning, TEXT("%s 的生命值发生变化, 当前生命值：%f"), *Props.TargetAvatarActor->GetName(), GetHealth())
	}
	//最大法力值
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	//元属性计算
	//传入伤害元属性
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	//传入XP元属性
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
	
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	//伤害计算
	if (LocalIncomingDamage > 0.f && GetHealth() > 0.f)
	{
		//修改血量
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		//致命伤害
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			//角色死亡
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				FVector Impulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			//发送经验事件
			SendXPEvent(Props);
		}
		else
		{
			{
				// 如果角色不处于被持续攻击状态
				if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
				{
					//受击反应
					FGameplayTagContainer TagContainer; //标签容器
					TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
					//根据标签激活能力
					Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
				}

				// 应用攻击击退值
				const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
				if (!KnockbackForce.IsNearlyZero(1.f)) // 如果击退的值不接近于0，则触发击退效果
				{
					Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
				}
			}
				
			//判断是否成功格挡
			const bool bBlockHit = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
			//判断是否成功暴击
			const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			//伤害文本
			ShowFloatingText(Props, LocalIncomingDamage, bBlockHit, bCriticalHit);
				
			// 判断Debuff是否生效
			if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
			{
				// Handle Debuff
				Debuff(Props);
			}
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	//UE_LOG(LogAura, Log, TEXT("传入XP：%f"), LocalIncomingXP);
		
	//如果实现玩家，战斗接口
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		//获取当前等级	
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		//获取当前XP
		const int32 CurrentXP = IPlayerInterface::Execute_GetPlayerXP(Props.SourceCharacter);
			
		//通过XP查找等级
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		//升级次数 2级-1级=1次
		const int32 NumOfLevelUps = NewLevel - CurrentLevel;
		if (NumOfLevelUps > 0)
		{
			//增加玩家等级
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumOfLevelUps);
			
			// 重置要奖励的可用属性点和技能点
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			// 累计连续升级总共要奖励多少可用属性点和技能点
			for (int32 i = 0; i < NumOfLevelUps; ++i)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			
			//增加可用属性点和可用技能点
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			//将当前血量和蓝量回满
			bTopOffHealth = true;
			bTopOffMana = true;

			//升级
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		//增加经验
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	// 获取Debuff相关参数-------------------------------------------------------------------------------------------------
	// 获取游戏标签结构体
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	// 获取伤害类型游戏标签
	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	// 获取伤害
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	// 获取持续时间
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	// 获取频率
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	// DebuffName 以伤害类型标签命名---------------------------------------------------------------------------------------
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	// 创建可实例化的GE
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	// 设置动态创建GE的属性------------------------------------------------------------------------------------------------
	// 设置时间策略
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration; // 持续时间策略
	// 设置周期
	Effect->Period = DebuffFrequency;
	// 设置持续时间幅度
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	// Effect->bExecutePeriodicEffectOnApplication = false; // 在应用后不会立即触发，而是在经过了Period后才会触发
	// Effect->PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset; // 设置每次应用后不会重置触发时间

	// 设置堆叠样式，就是可叠加层数
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource; // 按源聚合
	// 设置堆栈限制计数
	Effect->StackLimitCount = 1;
	
	// Effect->StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication; // 在应用后重置时，重置持续时间
	// Effect->StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::ResetOnSuccessfulApplication; // 在应用时，触发并重置Period时间
	// Effect->StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack; // GE时间到了默认清除所有层数，还有可以清除单层的设置
	// Effect->OverflowEffects.Add() // 在叠加层数超出时，将触发此数组内的GE应用到角色
	// Effect->bDenyOverflowApplication = true; // 设置为true时，叠加层数超出时，将不会刷新GE实例
	// Effect->bClearStackOnOverflow = true; // 设置为true时，叠加层数超出时，将清除GE

	// 修饰符 可以添加多个
	const int32 Index = Effect->Modifiers.Num(); // 获取当前修改属性的Modifiers的长度，也就是下一个添加的modify的下标索引
	Effect->Modifiers.Add(FGameplayModifierInfo()); // 添加一个新的Modify
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index]; // 通过下标索引获取Modify
	// 设置伤害
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	// 设置运算符号
	ModifierInfo.ModifierOp = EGameplayModOp::Additive; // 加
	// 设置修改的属性
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();

	// 赋予目标Actor Debuff标签-------------------------------------------------------------------------------------------
	// Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuff[DamageType]); // 以前可以直接修改标签容器，UE5.3以后已弃用
	// UE5.3以后需要通过GEComponent来设置GE应用的标签，向目标Actor赋予标签
	/*
	UTargetTagsGameplayEffectComponent& TargetTagsGameplayEffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>(); 
	FInheritedTagContainer InheritableOwnedTagsContainer = TargetTagsGameplayEffectComponent.GetConfiguredTargetTagChanges(); // 获取标签容器
	InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuff[DamageType]); // 添加标签
	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(InheritableOwnedTagsContainer); // 应用并更新
	*/

	UTargetTagsGameplayEffectComponent& TargetTagsGameplayEffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritableOwnedTagsContainer; // 创建组件所需的标签容器 而不是直接获取标签容器
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuff[DamageType];
	InheritableOwnedTagsContainer.Added.AddTag(DebuffTag); // 添加标签 添加的地方需要设置到Added对象上

	// 眩晕状态添加禁止输入标签
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		InheritableOwnedTagsContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		InheritableOwnedTagsContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		InheritableOwnedTagsContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		InheritableOwnedTagsContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	
	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(InheritableOwnedTagsContainer); // 应用并更新

	// 创建GE实例--------------------------------------------------------------------------------------------------------
	// MakeEffectContext
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	// AddSourceObject
	EffectContext.AddSourceObject(Props.SourceAvatarActor);
	// 创建GE实例，并添加伤害类型标签，应用GE
	FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
	if (MutableSpec)
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);
		
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
	
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	//回满血
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	//回满蓝
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	//生成伤害数字
	//不显示自己对自己造成伤害
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		//获取本地玩家控制器
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	//检查目标是否实现战斗接口
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		//获取目标等级
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		//获取目标职业
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		//获取经验奖励值
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		//标签
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		//Payload，要发送的数据
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		//发送经验事件
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

//GE预测旧值

/*
 * 主要属性
 */

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

/*
 *	次要属性
 */

//最大生命值
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

//最大法力值
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

/*
 * 属性抗性
 */

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OnRep_FireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OnRep_FireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OnRep_LightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OnRep_LightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OnRep_ArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OnRep_ArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OnRep_PhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OnRep_PhysicalResistance);
}

/*
 * 重要属性
 */

//生命值
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//通知ABS正在复制一个属性，传入OldHealth旧值是为了后续回滚数值
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

//法力值
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

