// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h" 
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//get patrol points
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControllingPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if(PatrolPoints.Num() == 0){ 
		auto Name = FString(ControllingPawn->GetName());
		UE_LOG(LogTemp, Warning, TEXT("No Patrol Points in Patrol Route for  %s"), *Name)
		return EBTNodeResult::Failed;
	}

	//set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// TODO, protect against empty patrol routes

	//increment waypoint
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	//UE_LOG(LogTemp,Warning,TEXT("Waypoint index: %i"), IndexKey)

	return EBTNodeResult::Succeeded;
}

