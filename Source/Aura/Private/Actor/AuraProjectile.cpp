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

	// 移动开启复制
	SetReplicateMovement(true);
	
	//触发重叠时回调
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	//移动音效组件
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	
}

void AAuraProjectile::OnHit()
{
	//播放声音
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	//播放特效
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	//停止播放移动音效
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

//销毁
void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
    {
    	LoopingSoundComponent->Stop();
    	LoopingSoundComponent->DestroyComponent();
    }
	//如果没有权限，并且bHit没有修改为true，则当前没有触发重叠事件，当销毁前播放声音和特效
	if (!bHit && !HasAuthority()) OnHit();
	
	Super::Destroyed();
}

//重叠
void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == NULL) return;
	//防止自己打自己
	AActor* SourceAvatarActor =  DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;

	//判断队伤
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;

	//没命中
	if (!bHit) OnHit();
	
	if (HasAuthority()) // 有权限
	{
		//对目标应用GE
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// 死亡击飞方向 设置为技能朝向
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			// 攻击击退概率
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				// 攻击击退方向
				// const FVector KnockbackDirection = GetActorForwardVector().RotateAngleAxis(45.f, GetActorRightVector()); // 向上45度
				// const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f; // 向上45度
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			
			// 设置目标ASC
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy(); //销毁自身
	}
	else bHit = true;
}

