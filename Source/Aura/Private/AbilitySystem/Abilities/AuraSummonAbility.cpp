// copyright


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	//获取召唤师的前向量
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	//获取召唤师的位置
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	//根据数量将召唤角度范围平均分段
	const float DeltaSpread = SpawnSpread / (NumMinions -1.f);

	//最左侧的角度边界
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);

	//最右侧的角度边界
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	//遍历每个分段
	for (int32 i = 0; i < NumMinions; i++)
	{
		//从左边界开始
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);

		//随机最远最近距离之间位置
		FVector ChosenSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		//地面线性跟踪
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		//DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f,12,FColor::Green,false, 3.f);

		SpawnLocations.Add(ChosenSpawnLocation);
		
	}

	
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
