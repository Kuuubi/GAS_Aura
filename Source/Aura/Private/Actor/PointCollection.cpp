// copyright


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	// 创建Pt_0作为根节点，中心点
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);

	// 将其它的场景组件作为根组件的子节点添加，会在蓝图中去修改它的位置
	CreatePointSceneComponent(Pt_1, "Pt_1");
	CreatePointSceneComponent(Pt_2, "Pt_2");
	CreatePointSceneComponent(Pt_3, "Pt_3");
	CreatePointSceneComponent(Pt_4, "Pt_4");
	CreatePointSceneComponent(Pt_5, "Pt_5");
	CreatePointSceneComponent(Pt_6, "Pt_6");
	CreatePointSceneComponent(Pt_7, "Pt_7");
	CreatePointSceneComponent(Pt_8, "Pt_8");
	CreatePointSceneComponent(Pt_9, "Pt_9");
	CreatePointSceneComponent(Pt_10, "Pt_10");
	
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("尝试访问索引不可超出数组范围"))

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;
		
		if (Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation(); // 中心点0到生成点1的向量
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector); // 旋转
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint); // 修改生成点的位置
		}

		// 升高位置作为射线起始点
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		// 降低位置作为射线结束点
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		// 获取到周围过滤的对象，这将要作为射线追踪要忽略的Actor
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayerWithRadius(
			this,
			IgnoreActors,
			TArray<AActor*>(),
			1500.f,
			GetActorLocation());

		// 创建射线，获取地面坐标
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors); // 要忽略的Actor
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);
		
		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation); // 修改生成的高度位置
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal)); // 与碰撞面垂直对齐

		//添加到返回数组内
		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

void APointCollection::CreatePointSceneComponent(TObjectPtr<USceneComponent>& Pt, FName PtName)
{
	Pt = CreateDefaultSubobject<USceneComponent>(PtName);
	ImmutablePts.Add(Pt);
	Pt->SetupAttachment(GetRootComponent());
}

