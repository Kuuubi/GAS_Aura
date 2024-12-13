// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

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

	/** Combat Interface */
	//获取受击反应蒙太奇动画
	virtual  UAnimMontage* GetHitReactMontage_Implementation() override;

	//角色死亡
	virtual void Die() override;
	
	//获取生成火球的骨骼插槽位置
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	//判断角色死亡
	virtual bool IsDead_Implementation() override;
	//获取Avatar
	virtual AActor* GetAvatar_Implementation() override;

	//获取攻击方式蒙太奇数组
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;

	//获取溅血特效
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;

	//根据标签获取攻击方式结构体
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;

	//获取召唤物数量计数
	virtual int32 GetMinionCount_Implementation() override;

	//召唤物数量计数增加
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;

	//获取职业类型
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	/** end Combat Interface */
	
	//多播死亡
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	//攻击方式结构体数组
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	virtual void BeginPlay() override;

	//Weapon骨骼网格组件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//用于武器攻击的骨骼插槽
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	//用于左手攻击的骨骼插槽
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandTipSocketName;

	//用于右手攻击的骨骼插槽
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandTipSocketName;

	//用于尾巴攻击的骨骼插槽
	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailTipSocketName;
	
	//角色是否死亡
	bool bDead = false;

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
	virtual void InitializeDefaultAttribute() const;

	//添加角色能力
	void AddCharacterAbilities();

	/* Dissolve Effects */

	//溶解函数
	void Dissolve();

	//溶解时间轴
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	//角色溶解材质实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	//武器溶解材质实例
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	//溅血奶瓜特效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;

	//死亡音效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;

	/* 召唤 */
	//召唤物数量计数
	int32 MinionCount = 0;

	//敌人职业
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

private:

	//主动能力列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//被动能力列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	//设置受击反应蒙太奇动画
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
