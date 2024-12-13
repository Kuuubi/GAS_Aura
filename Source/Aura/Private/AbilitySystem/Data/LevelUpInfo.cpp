// copyright


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		//索引[0]为空数据，索引[1]表示0-1级，[2]为1级，如果遍历整个数组，说明当前已是最高等级，直接返回等级
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		 if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		 {
		 	//当前400XP，1级对应要求是300XP，自增就到2级
			 ++Level;
		 }
		 else
		 {
		 	bSearching = false;
		 }
	}
	return Level;
}
