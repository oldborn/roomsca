// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMSCA_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Variables:
	UPROPERTY(EditAnywhere)
	float ReachDistance = 100.0f;
	APlayerController* PlayerController = nullptr;
	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;
	UInputComponent* InputComponent = nullptr;
	// Functions:
	bool FindFirstHit(FHitResult &HitResult, FVector &ReachPointLocation);
	FVector CalculateReach(FVector PlayerViewPointLocation, FRotator PlayerViewPointRotation);
	FVector CalculateReach();
	void Release();
	void Grab();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

