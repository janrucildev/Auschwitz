// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsOpen = false;
	IsReady = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = DoorFrame;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Druecker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Druecker"));
	Druecker->SetupAttachment(Door);
	Door->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	DoorRotateValue = 1.0f;
	DrueckerRotateValue = 1.0f;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, FName("ControlDoor"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

	if (DoorCurve && DrueckerCurve)
	{
		Timeline.AddInterpFloat(DoorCurve, TimelineCallback);
		Timeline.AddInterpFloat(DrueckerCurve, TimelineCallback);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);

}

void ADoor::ControlDoor()
{
	TimelineValue = Timeline.GetPlaybackPosition();

	//rotate door 
	DoorCurveFloatValue = DoorRotateValue * DoorCurve->GetFloatValue(TimelineValue);
	FQuat BlattRotation = FQuat(FRotator(0.0f, DoorCurveFloatValue, 0.0f));
	Door->SetRelativeRotation(BlattRotation);

	//rotate T�rdrucker
	DrueckerCurveFloatValue = DrueckerRotateValue * DrueckerCurve->GetFloatValue(TimelineValue);
	FQuat DrueckerRotation = FQuat(FRotator(0.0f, 0.0f, -DrueckerCurveFloatValue));
	Druecker->SetRelativeRotation(DrueckerRotation);
}

void ADoor::ToggleDoor()
{
	if (IsReady)
	{
		IsOpen = !IsOpen;
		DoorRotator = Door->GetRelativeRotation();
		DrueckerRotator = Druecker->GetRelativeRotation();

		if (IsOpen)
		{
			
			DoorRotateValue = 1.0f;
			

			//drücker
			DrueckerRotateValue = 1.0f;


			IsReady = false;
			UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
			Timeline.PlayFromStart();
		}
		else
		{
			

			IsReady = false;
			Timeline.Reverse();

			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, [this]()
				{
					UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
				}, 1, 0);
		}
	}
}

void ADoor::SetState()
{
	IsReady = true;
}

