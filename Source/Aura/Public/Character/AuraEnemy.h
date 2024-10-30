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
	/** Enemy Interface */

protected:
	virtual  void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
};
