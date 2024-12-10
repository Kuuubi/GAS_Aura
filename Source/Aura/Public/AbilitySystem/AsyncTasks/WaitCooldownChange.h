// copyright

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	//冷却时间开始
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	//冷却时间结束
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	//结束任务
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	//ASC
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	//记录监听的冷却标签
	FGameplayTag CooldownTag;

	//监听冷却标签变化回调
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	//监听ASC激活持续GE的回调
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
