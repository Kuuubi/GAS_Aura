// copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	//碰撞预设
	AAuraEnemy();
	//Pawn被控制器控制时触发回调
	virtual void PossessedBy(AController* NewController) override;

	/** Enemy Interface */
	//高亮Actor
	virtual void HighlightActoer() override;
	//取消高亮
	virtual void UnHighlightActoer() override;
	//设置敌人目标
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	//获取敌人目标
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Enemy Interface */

	/** Combat Interface */
	//获取敌人等级
	virtual int32 GetPlayerLevel_Implementation() override;;
	//角色死亡
	virtual void Die() override;
	/** end Combat Interface */

	//委托血量
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	//受击反馈回调
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	//是否被击中
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	//设置移动速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	//设置死亡后存在时间
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	//战斗目标
	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;
protected:
	virtual  void BeginPlay() override;
	
	//初始化AbilityActorInfo
	virtual void InitAbilityActorInfo() override;

	//初始化属性值
	virtual void InitializeDefaultAttribute() const override;

	//敌人等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	//血量条
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	//行为树
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	//AI控制器
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
};
