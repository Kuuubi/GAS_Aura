// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();

	// 被动技能特效标签
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;
	
	/**
 	 * 监听被动技能是否激活后的委托回调
 	 * @param AbilityTag 对应的技能的标签
 	 * @param bActivate 激活还是关闭
 	 */
	void OnPassiveActive(const FGameplayTag& AbilityTag, bool bActivate);
	
};
