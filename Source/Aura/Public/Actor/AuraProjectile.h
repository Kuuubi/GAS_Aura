// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

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

protected:
	virtual void BeginPlay() override;
	//球体重叠回调
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor *OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	//球体组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

};
