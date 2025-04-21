// copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
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

	virtual void Tick(float DeltaTime) override;
	
	// 重载复制属性函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//GetASC
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//GetAS
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	/**
 	 * 重载TakeDamage
 	 * @see https://www.unrealengine.com/blog/damage-in-ue4
 	 * @param DamageAmount		要施加的伤害数值
 	 * @param DamageEvent		描述伤害细节的结构体，支持不同类型的伤害，如普通伤害、点伤害（FPointDamageEvent）、范围伤害（FRadialDamageEvent）等。
 	 * @param EventInstigator	负责造成伤害的 Controller，通常是玩家或 AI 的控制器。
 	 * @param DamageCauser		直接造成伤害的 Actor，例如爆炸物、子弹或掉落的石头。
 	 * @return					返回实际应用的伤害值。这允许目标修改或减少伤害，然后将最终的值返回。
 	 */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Combat Interface */
	//获取受击反应蒙太奇动画
	virtual  UAnimMontage* GetHitReactMontage_Implementation() override;

	//角色死亡
	virtual void Die(const FVector& DeathImpulse) override;
	
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

	// 获取ASC注册成功委托
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;

	// 获取角色死亡委托
	virtual FOnDeathSignature& GetOnDeathDelegate() override;

	// 获取武器骨骼网格体
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;

	// 设置角色是否被持续攻击状态
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;

	// 获取角色是否被持续攻击状态
	virtual bool IsBeingShocked_Implementation() override;

	virtual FOnDamageSignature& GetOnDamageSignature() override;
	/** end Combat Interface */

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	//多播死亡
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	//攻击方式结构体数组
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	// 角色是否为被眩晕状态
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	// 角色是否为被点燃状态
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	// 角色是否为被持续攻击状态
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	// 眩晕状态复制通知回调
	UFUNCTION()
	virtual void OnRep_Stunned();

	// 点燃状态复制通知回调
	UFUNCTION()
	virtual void OnRep_Burned();

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

	// 眩晕标签变化后的回调
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// 设置移动速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY()
	//技能系统组件
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	//属性集
	TObjectPtr<UAttributeSet> AttributeSet;

	//初始化ActorInfo
	virtual void InitAbilityActorInfo();
	
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

	// 火焰Debuff奶瓜特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	// 眩晕Debuff奶瓜特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

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

	// 守护光环被动奶瓜特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	// 回复生命被动奶瓜特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	// 回复蓝量被动奶瓜特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	// 被动特效场景组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
