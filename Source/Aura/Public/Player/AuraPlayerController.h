// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct  FInputActionValue;
class IEnemyInterface;

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
	TObjectPtr<IEnemyInterface>  LastActor;
	TObjectPtr<IEnemyInterface>  ThisActor;
};
