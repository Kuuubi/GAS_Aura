// copyright


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//创建武器骨骼网络组件
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//添加武器网格插槽
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	//武器没有任何碰撞
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

