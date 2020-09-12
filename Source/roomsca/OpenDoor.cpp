// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	FRotator ActorRotator = GetOwner()->GetActorRotation();
	InitialYaw = ActorRotator.Yaw;
	OpenYaw = InitialYaw + 90.0f;
	TargetYaw = InitialYaw;
	if(!OpenDoorTV) UE_LOG(LogTemp, Warning,TEXT("OpenDoorTV is NULL in Actor: %s"), *GetOwner()->GetName());
	// ...
	
}


void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator ActorRotator = GetOwner()->GetActorRotation();
	ActorRotator.Yaw = FMath::Lerp(ActorRotator.Yaw, TargetYaw, DeltaTime*3.0f);
	GetOwner()->SetActorRotation(ActorRotator);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(IsTriggerContainsAnyActor())
	{
		LastOpenTime = GetWorld()->GetTimeSeconds();
		TargetYaw=OpenYaw;
	}else if(IsOpenForMaxSeconds())
	{
		TargetYaw=InitialYaw;
	}
	OpenDoor(DeltaTime);
	// ...
}

bool UOpenDoor::IsTriggerContainsAnyActor() const
{
	if(!OpenDoorTV) return false;
	TArray<AActor*> OverLappingActors;
	OpenDoorTV->GetOverlappingActors(OverLappingActors);
	if(OverLappingActors.Num() > 0) return true;
	return false;
}

bool UOpenDoor::IsTriggerContainsAnyPlayerActor() const
{
	if(!OpenDoorTV) return false;
	TArray<AActor*> OverLappingActors;
	OpenDoorTV->GetOverlappingActors(OverLappingActors);
	auto PlayerControllerIterator = GetWorld()->GetPlayerControllerIterator();
	while (PlayerControllerIterator)
	{
		UE_LOG(LogTemp, Warning, TEXT("While loop PlayerControllerIterator, OverLappingActors: %d"),OverLappingActors.Num());
		if(OverLappingActors.Contains(PlayerControllerIterator->Get()->GetPawn())) return true;
		++PlayerControllerIterator;
	}
	return false;
}

bool UOpenDoor::IsOpenForMaxSeconds() const
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	return LastOpenTime > 0.0f && (CurrentTime-LastOpenTime) > MaxOpenTimeInSeconds;
}


