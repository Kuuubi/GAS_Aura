// copyright

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:	
	AAuraProjectile();

	//抛体移动组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	//设置伤害GE
	UPROPERTY(BlueprintReadWrite,  meta = (ExposeOnSpawn = true))
	// FGameplayEffectSpecHandle DamageEffectSpecHandle;
	FDamageEffectParams DamageEffectParams;

	//追踪目标场景组件
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	//销毁
	virtual void Destroyed() override;
	
	//球体重叠回调
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor *OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//球体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	// 判断重叠有效性
	bool IsValidOverlap(AActor* OtherActor);
	
	//是否命中
	bool bHit = false;

	//火球移动音效组件
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
private:

	//火球存在时间
	UPROPERTY(EditAnywhere)
	float LifeSpan = 15.f;

	//击中奶瓜特效
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	//击中音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	//火球移动音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;;


};
