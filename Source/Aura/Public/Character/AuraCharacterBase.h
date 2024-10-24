// copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	//属性说明符
	UPROPERTY(EditAnywhere, Category="Combat")
	//名为Weapon的骨骼网格组件
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
