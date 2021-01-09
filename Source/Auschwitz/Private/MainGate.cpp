// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGate.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainGate::AMainGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsOpen = false;
	IsReady = true;

	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = BaseComp;

	LeftWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing"));
	LeftWing->SetupAttachment(RootComponent);

	RightWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing"));
	RightWing->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainGate::BeginPlay()
{
	Super::BeginPlay();

	WingsRotateValue = 1.0f;

	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;
	TimelineCallback.BindUFunction(this, FName("Control"));
	TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

	if (LiftCurve)
	{
		Timeline.AddInterpFloat(LiftCurve, TimelineCallback);
		Timeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	
}

// Called every frame
void AMainGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);

}

void AMainGate::Control()
{
	TimelineValue = Timeline.GetPlaybackPosition();

	//rotate door 
	WingsCurveFloatValue = WingsRotateValue * LiftCurve->GetFloatValue(TimelineValue);
	FQuat RightWingRotation = FQuat(FRotator(0.0f, WingsCurveFloatValue, 0.0f));
	FQuat LeftWingRotation = FQuat(FRotator(0.0f, -WingsCurveFloatValue, 0.0f));
	LeftWing->SetRelativeRotation(LeftWingRotation);
	RightWing->SetRelativeRotation(RightWingRotation);
}

void AMainGate::Toggle()
{
	if (IsReady)
	{
		IsOpen = !IsOpen;
		WingsRotator = LeftWing->GetRelativeRotation();

		if (IsOpen)
		{
			WingsRotateValue = 1.0f;

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

void AMainGate::SetState()
{
	IsReady = true;
}

