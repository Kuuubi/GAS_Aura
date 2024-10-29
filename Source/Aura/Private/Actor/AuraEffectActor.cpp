// copyright


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//设置根组件
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}



void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	/*获取ASC方法一
	 //检查目标是否实现ASC接口
	 IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Target);
	 if (ASCInterface)
	 {
	 //获取ASC
	 ASCInterface->GetAbilitySystemComponent();
	 }
	 */

	//获取ASC方法二
	//通过使用静态函数库，就像static_math数学库以及像那样的静态函数库
	//称为UAbilitySystemBlueprintLibrary
	//它会返回一个ASC
	UAbilitySystemComponent* TargetASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if ( TargetASC == nullptr) return;

	//检查设置效果类
	check(GameplayEffectClass);
	
	//游戏效果
	//EffectContextHandle
	FGameplayEffectContextHandle EffectContextHandle =  TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	//EffectSpecHandle
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f, EffectContextHandle);

	//应用游戏效果
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	//检查持续时间策略
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//添加键值对
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

//开始重叠函数
void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	//即时效果应用策略
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor , InstantGameplayEffectClass);
	}
	//持续效果应用策略
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor , DurationGameplayEffectClass);
	}
	//无限效果应用策略
	if(InfiniteEffectApplicationPolicy  == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor , InfiniteGameplayEffectClass);
	}
}

//结束重叠函数
void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstantEffectApplicationPolicy  == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor , InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor , DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy  == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor , InfiniteGameplayEffectClass);
	}
	
	//无限效果移除策略
	if(InfiniteEEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		//获取ASC
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		//遍历Map
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>  HandlePair : ActiveEffectHandles)
		{
			if (TargetASC ==  HandlePair.Value)
			{
				//移除游戏效果
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		//循环数组
		for(FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

