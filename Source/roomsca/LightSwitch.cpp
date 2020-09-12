// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitch.h"

// Sets default values for this component's properties
ULightSwitch::ULightSwitch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightSwitch::BeginPlay()
{
	Super::BeginPlay();
	if(!SwitchTV) UE_LOG(LogTemp, Warning, TEXT("Object (%s) does not have any TV attached to."), *GetOwner()->GetName());
	// ...
	
}


// Called every frame
void ULightSwitch::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SwitchLight();
	if(!SpotLight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object (%s) does not have any SpotLight attached to."), *GetOwner()->GetName());
	}else
	{
		SpotLight->SetEnabled(IsOpen);
	}
	// ...
}

void ULightSwitch::SwitchLight()
{
	if(!SwitchTV)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object (%s) does not have any TV attached to."), *GetOwner()->GetName());
		return;
	} 
	if(!Key)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object (%s) does not have any Key attached to."), *GetOwner()->GetName());
		return;
	}
	
	if(!IsOpen && SwitchTV->IsOverlappingActor(Key))
	{
		IsOpen = true;
	}else if(IsOpen && !SwitchTV->IsOverlappingActor(Key)){
		IsOpen = false;
	}
		
	
}

