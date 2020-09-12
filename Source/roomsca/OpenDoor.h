// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"
#define DOOR_STATE int
#define OPENED 2
#define OPENING 1
#define CLOSING -1
#define CLOSED -2

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMSCA_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.0f;
	float InitialYaw = 0.0f;
	float OpenYaw = 0.0f;
	float LastOpenTime = -1.0f;
	DOOR_STATE DoorState = CLOSED;
	UAudioComponent* DoorOpenAudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float MaxOpenTimeInSeconds = 0.5f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* OpenDoorTV;
	void OpenDoor(float DeltaTime);
	bool IsTriggerContainsAnyPlayerActor() const;
	bool IsOpenForMaxSeconds() const;
	UAudioComponent* GetDoorOpenAudioComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsTriggerContainsAnyActor() const;
};
