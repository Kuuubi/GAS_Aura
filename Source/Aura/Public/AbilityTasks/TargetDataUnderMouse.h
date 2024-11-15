// copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	//创建蓝图节点
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	//蓝图输出引脚
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	//重写执行函数
	virtual  void Activate() override;

	//客户端发送数据到服务端
	void SendMouseCursorData();

	//当数据提交到服务器端后的回调
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
