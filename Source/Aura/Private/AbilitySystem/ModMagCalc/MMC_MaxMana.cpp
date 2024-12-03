// copyright


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	//捕获属性
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	//捕获目标
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//快照
	IntelligenceDef.bSnapshot = false;

	//添加到属性捕获数组
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//获取Source 和 Target Tag
	const FGameplayTagContainer* SourceTags =  Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags =  Spec.CapturedSourceTags.GetAggregatedTags();

	//聚合
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//局部智慧值
	float Intelligence = 0.f;
	//捕获属性
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters,Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	//获取等级
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
}
