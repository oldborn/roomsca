// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber Started!"));
	PlayerController = GetWorld()->GetFirstPlayerController();
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandleComponent) UE_LOG(LogTemp, Error, TEXT("The Owner [%s] does not have UPhysicsHandleComponent"), *GetOwner()->GetName());
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("The Owner [%s] does not have UInputComponent"), *GetOwner()->GetName());
	}else
	{
		InputComponent->BindAction("GrabAction", IE_Pressed, this,&UGrabber::Grab);
		InputComponent->BindAction("GrabAction", IE_Released, this,&UGrabber::Release);
	}
	// ...
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if there is a component that is grabbed
	if (PhysicsHandleComponent->GrabbedComponent)
	{	
		PhysicsHandleComponent->SetTargetLocation(CalculateReach());
	}
	// ...
}


FVector UGrabber::CalculateReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation; 
	PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return CalculateReach(PlayerViewPointLocation, PlayerViewPointRotation);
}

FVector UGrabber::CalculateReach(FVector PlayerViewPointLocation, FRotator PlayerViewPointRotation)
{
	FVector ReachPointLocation = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*ReachDistance;
	return ReachPointLocation;
}

bool UGrabber::FindFirstHit(FHitResult &HitResult, FVector &ReachPointLocation)
{
	if(!PlayerController) PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation; 
	PlayerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	ReachPointLocation = CalculateReach(PlayerViewPointLocation, PlayerViewPointRotation);
	FCollisionQueryParams CollisionObjectQueryParams(
        FName(TEXT("")),
        false,
        GetOwner()
        );
	
	if(
        GetWorld()->LineTraceSingleByObjectType(
        HitResult,
        PlayerViewPointLocation,
        ReachPointLocation,
        FCollisionObjectQueryParams(ECC_PhysicsBody),
        CollisionObjectQueryParams
        )
    )
	{
		UE_LOG(LogTemp, Warning, TEXT("A hit to %s !"), *HitResult.Actor->GetName());
		return true;
	}
	return false;
}

void UGrabber::Grab()
{
	FVector ReachVector;
	FHitResult HitResult;
	if(FindFirstHit(HitResult, ReachVector))
	{
		UE_LOG(LogTemp, Warning, TEXT("Grab Started"));
		if(PhysicsHandleComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Grab Started"));
			PhysicsHandleComponent->GrabComponentAtLocation(HitResult.GetComponent(), NAME_None, ReachVector);
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("The Owner [%s] does not have UPhysicsHandleComponent"), *GetOwner()->GetName());
		}
	}
}

void UGrabber::Release()
{
	if(PhysicsHandleComponent)
	{
		PhysicsHandleComponent->ReleaseComponent();
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("The Owner [%s] does not have UPhysicsHandleComponent"), *GetOwner()->GetName());
	}
}




