// copyright


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::	FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	//查找标签
	for( const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT(
			"Cant find Info for AttributeTag [%s] on AttributeInfo [%s]."),
			*AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
