// copyright


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	//设置对应参数
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

//广播初始值函数
void UAuraWidgetController::BroadcastInitialValues()
{
}

//绑定回调到依赖项
void UAuraWidgetController::BindCallbacksToDependencies()
{
}
