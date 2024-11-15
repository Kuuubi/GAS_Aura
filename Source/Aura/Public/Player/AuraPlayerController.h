// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FGameplayTag;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct  FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
protected:
	//重写BeginPlay
	virtual void BeginPlay() override;
	//重写设置输入组件
	virtual  void SetupInputComponent() override;
	//重写PlayerTick
	virtual void PlayerTick(float DeltaTime) override;
private:
	//属性
	UPROPERTY(EditAnywhere, Category="Input")
	//添加输入映射上下文
	TObjectPtr<UInputMappingContext> AuraContext;

	//属性
	UPROPERTY(EditAnywhere, Category="Input")
	//添加输入操作
	TObjectPtr<UInputAction> MoveAction;

	//移动函数
	//输入操作需要绑定一个函数
	//函数必须传入FInputActionValue值
	void Move(const struct  FInputActionValue& InputActionValue);

	//鼠标跟踪函数
	void CursorTrace();
	IEnemyInterface*  LastActor;
	IEnemyInterface*  ThisActor;
	FHitResult CursorHit;

	//绑定输入回调函数
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//蓝图设置输入配置数据资产
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	//能力系统组件
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	///第一次访问时设置它，获取ASC
	UAuraAbilitySystemComponent* GetASC();

	//缓存点击目的地位置;
	FVector CachedDestination = FVector::ZeroVector;
	//追踪鼠标按下多久
	float FollowTime = 0.f;
	//短按阈值
	float ShortPressThreshold = 0.5f;
	//是否开启自动寻路
	bool bAutoRunning = false;
	//是否正在瞄准敌人
	bool bTargeting = false;
	
	//自动寻路半径
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	//样条曲线
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	//自动寻路
	void AutoRun();
};
