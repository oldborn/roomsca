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

UAudioComponent* UOpenDoor::GetDoorOpenAudioComponent()
{
	if(!DoorOpenAudioComponent) this->DoorOpenAudioComponent =  GetOwner()->FindComponentByClass<UAudioComponent>();
	return DoorOpenAudioComponent;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	FRotator ActorRotator = GetOwner()->GetActorRotation();
	InitialYaw = ActorRotator.Yaw;
	OpenYaw = InitialYaw + 90.0f;
	TargetYaw = InitialYaw;
	this->DoorOpenAudioComponent =  GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!DoorOpenAudioComponent) UE_LOG(LogTemp, Warning,TEXT("Missing Audio Component in: %s"), *GetOwner()->GetName());
	if(!OpenDoorTV) UE_LOG(LogTemp, Warning,TEXT("OpenDoorTV is NULL in Actor: %s"), *GetOwner()->GetName());
	
	// ...
	
}


void UOpenDoor::OpenDoor(float DeltaTime)
{
	if(DoorState == CLOSED && IsTriggerContainsAnyActor()){
		UE_LOG(LogTemp, Warning, TEXT("Door Opening..."));
		LastOpenTime = GetWorld()->GetTimeSeconds();
		TargetYaw=OpenYaw;
		DoorState = OPENING;
		UAudioComponent* Audio = GetDoorOpenAudioComponent();
		if(Audio) Audio->Play();
	}else if(DoorState == OPENED && IsOpenForMaxSeconds() && !IsTriggerContainsAnyActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Door Closing..."));
		TargetYaw=InitialYaw;
		DoorState = CLOSING;
		UAudioComponent* Audio = GetDoorOpenAudioComponent();
		if(Audio) Audio->Play();
	}
	
	FRotator ActorRotator = GetOwner()->GetActorRotation();
	if(abs(ActorRotator.Yaw - TargetYaw) < 0.1f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door is Stable..."));
		if(TargetYaw == InitialYaw)
		{
			DoorState = CLOSED;
		}else
		{
			DoorState = OPENED;
		}
	}
	ActorRotator.Yaw = FMath::Lerp(ActorRotator.Yaw, TargetYaw, DeltaTime*3.0f);
	GetOwner()->SetActorRotation(ActorRotator);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
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


