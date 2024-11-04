// copyright


#include "UI/HUD/AuraHUD.h"

#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

//创建或返回Overlay控制器
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		//创建控制器，其类为蓝图中设置的Overlay组件控制器类
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,  OverlayWidgetControllerClass );
		//调用控制器父类里的设置对应参数函数
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//调用绑定回调到依赖函数，属性值变化时
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		//使用新对象为蓝图设置的AttributeMenu组件控制器类创建组件控制器
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this,  AttributeMenuWidgetControllerClass );
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		//绑定监听数值变化
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

//初始化Overlay
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class 未初始化，请填写BP_AuraHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class 未初始化，请填写BP_AuraHUD"))
	
	//创建Widget，其类为蓝图中设置的OverlayWidget类
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	//构建Overlay组件控制器
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	//调用父类里的设置组件控制器函数
	OverlayWidget->SetWidgetController(WidgetController);
	//调用广播初始值函数
	WidgetController->BroadcastInitialValues();
	
	//添加到视口
	Widget->AddToViewport();
}
