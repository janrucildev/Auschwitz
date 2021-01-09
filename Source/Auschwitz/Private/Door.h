// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class ADoor : public AActor
{
	
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Door;


	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Druecker;

	//used to open/close the Druecker smoothly (animation)
	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorCurve;

	//used to open/close the door smoothly (animation)
	UPROPERTY(EditAnywhere)
		UCurveFloat* DrueckerCurve;

	//played when the door is opened
	UPROPERTY(EditAnywhere)
		class USoundCue* OpenSound;

	//played when the door is closed
	UPROPERTY(EditAnywhere)
		class USoundCue* CloseSound;

	//handles the actual logic
	UFUNCTION()
		void ControlDoor();

	//this is what the character calls
	UFUNCTION()
		void ToggleDoor();

	//determine if the door is ready or not
	UFUNCTION()
		void SetState();

	bool IsOpen;
	bool IsReady;

	FTimeline Timeline;
	float TimelineValue;

	FRotator DrueckerRotator;
	float DrueckerRotateValue;
	float DrueckerCurveFloatValue;

	FRotator DoorRotator;
	float DoorRotateValue;
	float DoorCurveFloatValue;
};
