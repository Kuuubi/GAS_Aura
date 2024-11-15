// copyright


#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	//开启复制
	bReplicates = true;
	
	//初始化球体组件
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	//设置为根组件
	SetRootComponent(Sphere);
	//设置碰撞为仅查询
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//设置碰撞响应通道
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);	//忽略所有碰撞
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//初始化抛体移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	//设置初始速度
	ProjectileMovement->InitialSpeed = 550.f;
	//设置最大移速
	ProjectileMovement->MaxSpeed = 550.f;
	//设置重力比例
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	//触发重叠时回调
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

