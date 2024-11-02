// copyright


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//指定要捕获的属性
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//确定从目标还是源捕获
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//快照
	VigorDef.bSnapshot = false;

	//将捕获的属性值添加到属性捕获的数组
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//获取 Source 和 Target tag
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//数据聚合器
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//局部活力属性值
	float Vigor = 0.0f;
	//捕获属性
	GetCapturedAttributeMagnitude(VigorDef,  Spec, EvaluateParameters, Vigor);
	//如果Vigor值小于0将其设置为0
	Vigor = FMath::Max<float>(Vigor, 0.0f);;

	//获取玩家等级
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	//自定义计算
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
	
}
