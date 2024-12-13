// copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT()
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	//每个等级要达到的XP值
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	//奖励属性点
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 0;

	//奖励技能点
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 0;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	//等级升级信息数组
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	//经验值找到对应等级
	int32 FindLevelForXP(int32 XP) const;
	
};
