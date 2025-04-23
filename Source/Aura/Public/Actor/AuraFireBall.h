// copyright

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectile.h"
#include "AuraFireBall.generated.h"


UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
	
public:

	// 执行蓝图时间轴事件，需要在蓝图中实现此事件
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	// 当前火球返回的目标角色，默认是技能的释放者，在创建火球时创建
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;
	
	// 伤害配置参数
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
protected:
	virtual void BeginPlay() override;
	// 重载球体重叠回调
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlapPrimitiveComponent, AActor *OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 重载命中
	virtual void OnHit() override;
	
};
