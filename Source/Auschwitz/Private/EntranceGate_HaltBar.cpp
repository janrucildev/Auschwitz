// Fill out your copyright notice in the Description page of Project Settings.


#include "EntranceGate_HaltBar.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEntranceGate_HaltBar::AEntranceGate_HaltBar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsOpen = false;
	IsReady = true;

	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = BaseComp;

	BarComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bar"));
	BarComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEntranceGate_HaltBar::BeginPlay()
{
	Super::BeginPlay();

	BarRotateValue = 1.0f;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, FName("ControlDoor"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

	if (LiftCurve)
	{
		Timeline.AddInterpFloat(LiftCurve, TimelineCallback);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
	
}

// Called every frame
void AEntranceGate_HaltBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);

}

void AEntranceGate_HaltBar::ControlDoor()
{
	TimelineValue = Timeline.GetPlaybackPosition();

	//rotate door 
	BarCurveFloatValue = BarRotateValue * LiftCurve->GetFloatValue(TimelineValue);
	FQuat BlattRotation = FQuat(FRotator(0.0f, 0.0f, BarCurveFloatValue));
	BarComp->SetRelativeRotation(BlattRotation);
}

void AEntranceGate_HaltBar::Toggle()
{
	if (IsReady)
	{
		IsOpen = !IsOpen;
		BarRotation = BarComp->GetRelativeRotation();

		if (IsOpen)
		{
		
			BarRotateValue = -1.0f;

			IsReady = false;
			UGameplayStatics::PlaySoundAtLocation(this, LiftupSound, GetActorLocation());
			Timeline.PlayFromStart();
		}
		else
		{
			

			IsReady = false;
			Timeline.Reverse();

			FTimerHandle handle;
			GetWorld()->GetTimerManager().SetTimer(handle, [this]()
				{
					UGameplayStatics::PlaySoundAtLocation(this, LiftdownSound, GetActorLocation());
				}, 1, 0);
		}
	}
}

void AEntranceGate_HaltBar::SetState()
{
	IsReady = true;
}

