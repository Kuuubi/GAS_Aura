// copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

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

	/** Enemy Interface */
	//高亮Actor
	virtual void HighlightActoer() override;
	//取消高亮
	virtual void UnHighlightActoer() override;
	/** end Enemy Interface */

	/** Combat Interface */
	//获取敌人等级
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

protected:
	virtual  void BeginPlay() override;
	
	//初始化AbilityActorInfo
	virtual void InitAbilityActorInfo() override;

	//敌人等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
};
