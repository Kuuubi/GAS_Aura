// copyright


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

//捕获定义结构体
struct AuraDamageStatics
{
	//捕获宏声明指定捕获属性
	/* 次要属性 */
	//护甲
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	//护甲穿透
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	//格挡几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	//暴击几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	//暴击伤害
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	//暴击抵抗
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	/* 属性抗性 */
	//火属性抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	//雷属性抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	//魔法抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	//物理抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	//TMap标签对应捕获属性
	//TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		//捕获定义参数宏
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)
		
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//添加到属性捕获数组
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTagss = FAuraGameplayTags::Get();
	// 遍历所有的伤害类型对的Debuff
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : GameplayTagss.DamageTypesToDebuff)
	{
		const FGameplayTag& DamageType = Pair.Key;// 获取伤害类型
		const FGameplayTag& DebuffType = Pair.Value;// 获取对应的Debuff
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f); 
		// 如果Debuff设置了伤害，即使为0，也需要应用Debuff效果
		if (TypeDamage > -.5f)
		{
			// 获取造成Debuff几率
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTagss.Debuff_Chance, false, -1.f);

			// 捕获Debuff对应的抗性值
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceType = GameplayTagss.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceType], EvaluateParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f); // 抗性最低为0

			// 计算最终造成Debuff几率
			const float EffectiveDebuffChance = SourceDebuffChance * ( 100 - TargetDebuffResistance ) / 100.f;
			// 判断Debuff是否生效
			const float RandDebuff = FMath::RandRange(1, 100);
			bool bDebuff = RandDebuff < EffectiveDebuffChance;
			if (bDebuff)
			{
				// 获取GEctx 设置Debuff相关配置
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				// 设置Debuff生效
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				// 设置Debuff伤害类型
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				// GetSetByCallerMagnitude
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTagss.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTagss.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTagss.Debuff_Frequency, false, -1.f);
				// 设置Debuff伤害数值 持续时间 触发频率
				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				
			}
			
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// TMap标签对应的捕获属性
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	// TMap添加标签和对应捕获属性数据
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	//获取ASC
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	//获取Avatar
	//如果ASC有效就获取Avatar并设置为空指针
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	//检查是否实现战斗接口
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		//获取等级
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	
	//获取Spec信息
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	//获取Context
	FGameplayEffectContextHandle EffectContextHandle =  Spec.GetContext();

	//获取标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	//数值计算聚合器
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters, TagsToCaptureDefs);
		
	//获取伤害类型对应的伤害值
	float Damage = 0.f;
	//遍历伤害类型对应抗性TMap
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		
		//检查对应的捕获属性是否设置，防止崩溃
		checkf(TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("在ExecCalc_Damage中，无法获取Tag：[%s] 对应的捕获属性"), *ResistanceTypeTag.ToString());
		//通过TMap传入抗性标签获取对应的捕获属性
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTypeTag];

		//从 SetByCallerMagnitude 通过伤害类型标签找到对应的值
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, 0.f);

		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		//捕获抗性值
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		//抗性按百分比减少伤害
		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;

		// 范围伤害
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. 在 AuraCharacterBase 里重载 TakeDamage
			// 2. 在战斗接口创建 OnDamageDelegate 委托，在 TakeDamage 里广播伤害
			// 3. 在战斗接口声明一个虚函数返回&引用，并在角色基类实现，在计算伤害时从受到受害者上通过战斗接口获取到委托，并绑定lambda
			// 4. 调用 UGameplayStatics::ApplyRadialDamageWithFalloff 造成伤害(这将调用TakeDamage, 然后调用广播 OnDamageDelegate 委托)
			// 5. 在lambda中，设置 DamageTypeValue

			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});	
			}

			// 范围伤害衰减
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f, // 最小伤害为0，在伤害半径之外不会受到任何伤害
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f, // 线性伤害衰减
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr
				);
		}
		
		Damage += DamageTypeValue;
	}

	//计算捕获属性的数值
	/* 格挡几率 */
	//目标格挡几率
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
	//夹紧属性值
	TargetBlockChance = FMath::Max<float>( 0.0f, TargetBlockChance);
	//判断攻击是否被成功阻挡，如果阻挡成功，伤害将减半
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	//Context设置是否成功格挡
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	//格挡伤害计算
	Damage = bBlocked ? Damage / 2.f : Damage;

	/* 护甲，护甲穿透 */
	//目标护甲
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	//夹紧属性值
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	//来源护甲穿透
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	//夹紧属性值
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	//获取角色类信息数据资产
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	//查找护甲穿透曲线
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	//来源玩家等级对应的护甲穿透系数
	const float SourceArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	//护甲穿透会忽略目标护甲百分比
	//目标有效护甲
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * SourceArmorPenetrationCoefficients) / 100.f;

	//查找护甲曲线
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	//目标玩家等级对应的护甲系数
	const float TargetEffectiveArmorCoefficients = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	//伤害 * 目标护甲百分比忽略受到的伤害
	Damage *= ( 100 - EffectiveArmor * TargetEffectiveArmorCoefficients) / 100.f;

	/* 暴击几率，暴击伤害，暴击抵抗 */
	//来源暴击几率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	//来源暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	//目标暴击抵抗
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	//查找暴击抵抗曲线
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	//目标玩家等级对应的暴击抵抗系数
	const float TargetCriticalHitResistanceCoefficients = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	//来源有效暴击几率
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * TargetCriticalHitResistanceCoefficients;
	//判断暴击
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	//Context设置是否成功暴击
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	//暴击伤害计算
	Damage = bCriticalHit ? Damage * 2.f  + SourceCriticalHitDamage: Damage;
	
	
	//计算方式
	//const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	//输出计算结果
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
