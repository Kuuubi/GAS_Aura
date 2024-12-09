// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


//UI组件数据表结构
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	//初始化
	//游戏标签
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	//文本
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	
	FText Message = FText();

	//对任何给定的游戏标签，创建一个组件并将其添加到视口
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	//图像
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};

class UAbilityInfo;
class UAuraUserWidget;
class UAuraAbilitySystemComponent;

//声明代理类型，即委托签名
//属性
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float , NewValue);
//消息组件表行
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow , Row);
//技能信息数据
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo, Info);



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

	//绑定回调到依赖项函数
	virtual  void BindCallbacksToDependencies() override;

	//定义代理的广播属性值
	//生命值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	//法力值
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	//消息组件表行委托
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	//技能信息数据
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:
	//设置消息组件数据表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	//设置技能数据资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	//查找数据表
	template<typename  T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	//初始化拥有的技能回调
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent);
};

//查找数据表
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	//通过标签名称查找行的名称
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}