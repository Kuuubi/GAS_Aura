// copyright


#include "AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//检查当前Task是否由本地客户端控制
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//如果是客户端控制器控制，将数据发送到服务器端
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		//服务器监听TargetData
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//判断在服务器端，上面的委托是否已经广播过
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			//设置服务器端等待PlayerData数据的上传
			SetWaitingOnRemotePlayerData();
		}
	}
	
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	//创建预测窗口
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	//创建上传数据TargetData
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	
	//获取玩家控制器
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	//得到鼠标点击结果
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//TargetDataHandle
	FGameplayAbilityTargetDataHandle DataHandle;
	//将要上传的数据设置为鼠标点击结果
	Data->HitResult = CursorHit;
	//将Data添加到Handle
	DataHandle.Add(Data);
	
	//ASC调用上传函数
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
		);

	//如果服务端通过验证，本地客户端就广播数据
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//通知客户端 服务器端已经接收并处理了从客户端复制的目标数据（将服务器的TargetData应用到客户端，并清除掉缓存）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	//如果服务端通过验证，本地客户端就广播数据
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
