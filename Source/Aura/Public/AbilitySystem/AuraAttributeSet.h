// copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

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

	//生命值
	//BlueprintReadOnly设置蓝图只读
	//ReplicatedUsing为当服务端修改Health数值时的回调，拥有Replicated属性时，其值会从服务器自动复制到客户端
	//Category设置类别
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health,  Category = "Vital Attributes")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth,  Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	
		UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
		UFUNCTION()
		void Onrep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;


	//法力值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana,  Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana,  Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};