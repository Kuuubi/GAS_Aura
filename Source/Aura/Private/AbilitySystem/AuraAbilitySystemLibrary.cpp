// copyright


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	//获取组件控制器返回给调用者
	//获取本地玩家控制器，从玩家控制器就可以访问HUD
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		//获取HUD
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			//获取玩家状态
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			//获取ASC
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			//获取AS
			UAttributeSet* AS = PS->GetAttributeSet();
			//构造结构体
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			//构造组件控制器
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		//获取HUD
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			//获取玩家状态
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			//获取ASC
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			//获取AS
			UAttributeSet* AS = PS->GetAttributeSet();
			//构造结构体
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			//构造组件控制器
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
