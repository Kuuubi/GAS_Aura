// copyright

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
//属性信息委托
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	//绑定回调到依赖项
	virtual  void BindCallbacksToDependencies() override;
	//广播初始值
	virtual  void BroadcastInitialValues() override;

	//蓝图绑定属性信息委托
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	//属性信息
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	//广播属性信息
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
