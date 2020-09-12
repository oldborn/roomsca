// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/SpotLight.h"
#include "Engine/TriggerVolume.h"

#include "LightSwitch.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMSCA_API ULightSwitch : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULightSwitch();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* SwitchTV;
	UPROPERTY(EditAnywhere)
	AActor* Key;
	UPROPERTY(EditAnywhere)
	ASpotLight* SpotLight;
	bool IsOpen = true;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SwitchLight();
};
