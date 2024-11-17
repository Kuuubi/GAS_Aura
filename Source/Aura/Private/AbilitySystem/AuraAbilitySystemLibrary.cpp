// copyright


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	//获取游戏模式
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode ==nullptr)return;

	//获取AvatarActor用于添加源对象
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	//通过游戏模式获取设置的数据资产
	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	//通过职业枚举获取对应的主要属性
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	//应用主要属性GE
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	//添加源对象
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle =  ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	//应用次要属性GE
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	//添加源对象
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle =  ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	//应用重要属性GE
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	//添加源对象
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle =  ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(* VitalAttributesSpecHandle.Data.Get());
	
}
