// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	//创建OverlayWidget指针
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	//创建或返回Overlay控制器
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams );

	//初始化Overlay函数
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	//可以在蓝图中设置所需的Widget类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	//用于存储创建的Overlay控制器指针
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	//可以在蓝图中设置所需的Overlay控制器类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
