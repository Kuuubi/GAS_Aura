// copyright


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	//开启复制
	bReplicates = true;
	
	//初始化球体组件
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	//设置为根组件
	SetRootComponent(Sphere);
	//设置碰撞对象类型
	Sphere->SetCollisionObjectType(ECC_Projectile);
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

	//火球存在时间
	SetLifeSpan(LifeSpan);
	
	//触发重叠时回调
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	//移动音效组件
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	
}

//销毁
void AAuraProjectile::Destroyed()
{
	//如果没有权威，并且bHit没有修改为true，则当前没有触发重叠事件，当销毁前播放声音和特效
	if (!bHit && !HasAuthority())
	{
		//播放声音
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		//播放特效
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		//停止播放移动音效
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

//重叠
void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//防止自己打自己
	if (DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	//判断队伤
	if (!UAuraAbilitySystemLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}

	//没命中
	if (!bHit)
	{
		//播放声音
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		//播放特效
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		//停止播放移动音效
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}

	//销毁自身
	if (HasAuthority())
	{
		//对目标应用GE
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

