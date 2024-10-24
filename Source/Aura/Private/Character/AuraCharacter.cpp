// copyright


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	//True 将角色朝移动方向旋转
	GetCharacterMovement()->bOrientRotationToMovement =true;
	//旋转速率
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//True 角色约束在平面
	GetCharacterMovement()->bConstrainToPlane = true;
	//设置了上面一项为True，则此项设置为True，则开始与地面对齐
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	//角色不受控制器旋转
	//设置使用控制器选中Pitch，Yaw，Roll的值
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
