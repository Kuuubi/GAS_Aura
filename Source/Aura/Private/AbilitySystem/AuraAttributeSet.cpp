// copyright


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

//注册变量
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//设置Health属性在服务器上可以被Replicate，COND_复制条件有None，Always，Replicate
	//REPNOTIFY_默认值是OnChanged，也就是被更改时才会复制，Always始终即使不更改也会复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always)
DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)
}

//属性值变化前触发函数
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//变化前夹紧属性范围
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}


//属性影响结构体
void UAuraAttributeSet::SetFEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	//Source = 效果的来源，Target = 效果的目标
	
	//获取Source信息
	//EffectSpec
	Props.EffectContextHandle  = Data.EffectSpec.GetContext();
	
	//调用获取原始函数，获取来源的ASC
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	//访问控制器
	//判断有效
	//AbilityActorInfo是包装在T指针里的，先检查确保指针已存储后，包装器有效。则可以使用->运算符检查AvatarActor有效
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		//调用函数Get原始指针获取AvatarActor并存储
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		
		//现在可以获得例如，可以获取来源控制器，包装在弱指针里要Get
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		//但是如果ActorInfo是空指针，需要通过将其转换成Pawn来从AvatarActor本身获取
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			//换成Pawn
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				//源控制器=获取控制器
				Props.SourceController = Pawn->GetController();
			}
		}
		
		//如果源控制器不为空，可以得到Character的Pawn
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	
	//获取Target信息
	//检查目标AbilityActorInfo和AvatarActor有效性
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		//获取AvatarActor
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		//获取PlayerController
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		//AvatarActor转换成ACharacter
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		//获取Target的ASC
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


//属性值变化后触发函数
void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//创建结构体
	FEffectProperties Props;
	//将数据传给Props
	SetFEffectProperties(Data, Props);

	//变化后夹紧属性值
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.1, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.1, GetMaxMana()));
	}
	
}

//生命值
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//通知ABS正在复制一个属性，传入OldHealth旧值是为了后续回滚数值
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

//最大生命值
void UAuraAttributeSet::Onrep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

//法力值
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMana);
}

//最大法力值
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMaxMana);
}



