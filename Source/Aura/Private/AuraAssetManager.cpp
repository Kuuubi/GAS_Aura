// copyright


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine)
	//返回引擎资产管理器
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	//使用TargetData，必须开启此项
	UAbilitySystemGlobals::Get().InitGlobalData();
}
