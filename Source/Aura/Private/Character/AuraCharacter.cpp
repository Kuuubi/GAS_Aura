// copyright


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	//当设置为True时，角色将朝移动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement =true;
	//控制角色旋转速率
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//当设置为True时，角色移动约束在平面
	GetCharacterMovement()->bConstrainToPlane = true;
	//设置了上面一项为True，此项设置为True
	//则游戏开始时，角色被吸附到地面，防止有空中坠落，或卡在地面的问题
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//禁止相机随角色旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
