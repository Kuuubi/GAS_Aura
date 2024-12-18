// copyright

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class UNiagaraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public  IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter ();

	//ASC初始化
	//服务端
	virtual  void PossessedBy(AController* NewController) override;
	//客户端
	virtual  void OnRep_PlayerState() override;

	/** Player Interface */
	//通过XP查找当前等级
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	
	//获取当前经验值
	virtual int32 GetPlayerXP_Implementation() const override;

	//获取要奖励多少可用属性点
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;

	//获取要奖励多少可用技能点
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	
	//增加玩家经验值
	virtual void AddToXP_Implementation(int32 InXP) override;

	//增加玩家等级
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;

	//增加可用属性点
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	
	//增加可用技能点
	virtual void AddToSpellPointsReward_Implementation(int32 InSpellPointsReward) override;

	//升级
	virtual void LevelUp_Implementation() override;
	/** end Player Interface */

	/** Combat Interface */
	//获取玩家等级
	virtual int32 GetPlayerLevel_Implementation() override;;
	/** end Combat Interface */

	//升级奶瓜特效组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:
	//相机
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	//相机弹簧臂
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	//初始化AbilityActor
	virtual void InitAbilityActorInfo() override;

	//多播升级粒子特效
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
