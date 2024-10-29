// copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//结构宏
USTRUCT()
//属性影响结构体
struct FEffectProperties
{
	//正文宏
	GENERATED_BODY()

	FEffectProperties(){}

	//EffectContextHandle
	FGameplayEffectContextHandle EffectContextHandle;

	//指针，确保初始化null
	//Source
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	//Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//声明属性函数
	UAuraAttributeSet();
	//注册变量，指定哪些属性需要在服务器和客户端同步，并定义每个属性的复制条件
	virtual  void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//属性值变化前触发函数
	virtual  void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//属性值变化后触发函数
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//生命值
	//BlueprintReadOnly设置蓝图只读
	//ReplicatedUsing为当服务端修改Health数值时的回调，拥有Replicated属性时，其值会从服务器自动复制到客户端
	//Category设置类别
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,  Category = "Vital Attributes")
	FGameplayAttributeData Health;
	//生命值属性访问器
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,  Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	//法力值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana,  Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana,  Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	
		UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
		UFUNCTION()
		void Onrep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

		UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
		UFUNCTION()
		void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

protected:

	//设置属性影响结构体函数
	void SetFEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
