// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "EntranceGate_HaltBar.generated.h"

UCLASS()
class AEntranceGate_HaltBar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntranceGate_HaltBar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BaseComp;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* BarComp;



	//used to open/close the Druecker smoothly (animation)
	UPROPERTY(EditAnywhere)
		UCurveFloat* LiftCurve;


	//played when the door is opened
	UPROPERTY(EditAnywhere)
		class USoundCue* LiftupSound;

	//played when the door is closed
	UPROPERTY(EditAnywhere)
		class USoundCue* LiftdownSound;

	//handles the actual logic
	UFUNCTION()
		void ControlDoor();

	//this is what the character calls
	UFUNCTION()
		void Toggle();

	//determine if the door is ready or not
	UFUNCTION()
		void SetState();

	bool IsOpen;
	bool IsReady;

	FTimeline Timeline;
	float TimelineValue;



	FRotator BarRotation;
	float BarRotateValue;
	float BarCurveFloatValue;

};
