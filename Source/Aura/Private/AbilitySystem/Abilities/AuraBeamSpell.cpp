// copyright


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.Location;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		// 取消技能
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter)
	// 确保所有者继承了战斗接口
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		// 获取到武器
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			// 获取骨骼插槽
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));

			TArray<AActor*> ActorsToIgnore; // 当前需要忽略的对象数组
			ActorsToIgnore.Add(OwnerCharacter); // 将所有者角色忽略

			FHitResult HitResult; // 命中结果

			// 在武器插槽位置和鼠标点击位置生成一条球形线，获取第一个命中的目标
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None, //调试跟踪类型如果需要debug，将其设置ForDuration，如果关闭设置为None
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				// 将被阻挡位置设置为第一个目标
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}

			// 如果目标实现战斗接口
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
			{
				// 判断是否已经绑定了委托回调
				if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
				{
					// 未绑定则绑定主要目标死亡回调
					CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
				}
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore; // 遍历时忽略的数组
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo()); // 忽略自身
	ActorsToIgnore.Add(MouseHitActor); // 忽略鼠标点击的目标
	
	TArray<AActor*> OverlappingActors; // 存放遍历结果的数组

	// 获取到一定范围内的全部敌人
	UAuraAbilitySystemLibrary::GetLivePlayerWithRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	//int32 NumAdditionalTargets = MaxNumShockTargets;

	// 获取最近的几个目标
	UAuraAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, MouseHitActor->GetActorLocation());

	// 绑定额外目标死亡回调
	for (AActor* Target: OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}

}
