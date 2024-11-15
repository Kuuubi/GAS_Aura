// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public  IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	//GetASC
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//GetAS
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

protected:
	virtual void BeginPlay() override;

	//属性说明符
	UPROPERTY(EditAnywhere, Category="Combat")
	//名为Weapon的骨骼网格组件
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//用于施法的骨骼插槽
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	//获取生成火球的骨骼插槽位置
	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY()
	//技能系统组件
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	//属性集
	TObjectPtr<UAttributeSet> AttributeSet;

	//初始化ActorInfo
	virtual   void  InitAbilityActorInfo();
	
	//默认主要属性游戏效果
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	//默认次要属性游戏效果
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
    TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	//默认重要属性游戏效果
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	//应用游戏效果于属性
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttribute() const;

	//添加角色能力
	void AddCharacterAbilities();

private:

	//能力列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
};
