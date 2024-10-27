// copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

//声明委托类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeedSignature, float , NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangeedSignature, float , NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangeedSignature, float , NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangeedSignature, float , NewMaxMana);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//广播初始值函数
	virtual  void BroadcastInitialValues() override;

	//委托生命值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangeedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangeedSignature OnMaxHealthChanged;

	//委托法力值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangeedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangeedSignature OnMaxManaChanged;
};
