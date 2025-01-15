// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
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
	//创建或返回Overlay控制器
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams );
	
	//创建或返回AttributeMenu控制器
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams );

	//创建或返回SpellMenu控制器
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams );

	//初始化Overlay
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	//用于存储创建的OverlayWidget指针
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	//设置所需的OverlayWidget类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	//用于存储创建的Overlay控制层指针
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	//设置所需的Overlay控制层类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	//用于存储创建的AttributeMenu控制层指针
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	//设置所需的AttributeMenu控制层类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	//用于存储创建的SpellMenu控制层指针
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	
	//设置所需的SpellMenu控制层类
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
