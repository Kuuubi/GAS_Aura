// copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter ();

	//ASC初始化
	//服务端
	virtual  void PossessedBy(AController* NewController) override;
	//客户端
	virtual  void OnRep_PlayerState() override;

	/** Combat Interface */
	//获取玩家等级
	virtual int32 GetPlayerLevel() override;
	/** end Combat Interface */

private:
	virtual void InitAbilityActorInfo() override;
};
