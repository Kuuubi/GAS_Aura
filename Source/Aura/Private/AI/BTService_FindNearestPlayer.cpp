// copyright


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//通过AI控制器获取Pawn
	APawn* OwningPawn = AIOwner->GetPawn();

	//判断使用此服务Pawn的标签来得到目标的标签
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	
	//获取所有带有标签的Actor
	TArray<AActor*> ActorsWithTag;;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	//遍历Actor数组，获取最近玩家
	//最近距离设置为float最大值
	float ClosestDistance = TNumericLimits<float>::Max();
	//最近距离Actor
	AActor* ClosestActor = nullptr;
	for (auto Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			//循环每个Actor的距离
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	
	//设置黑板键数据
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
