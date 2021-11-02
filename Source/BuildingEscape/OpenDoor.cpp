// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialAngle = GetOwner()->GetActorRotation().Yaw;
	CurrentAngle = InitialAngle;
	OpenDoorAngle += InitialAngle;
	FindAudioComponent();

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor Component but no PressurePlate set."), *GetOwner()->GetName());
	}

}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No AudioComponent Found On: %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	if(TotalMassOfActors() >= TotalMassRequired)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentAngle = FMath::FInterpTo(CurrentAngle, OpenDoorAngle, DeltaTime, 1);
	OpenRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(OpenRotation);

	if(!AudioComponent) {return;}
	if(!DoorSoundPlayed)
	{
		AudioComponent->Play();
		DoorSoundPlayed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentAngle = FMath::FInterpTo(CurrentAngle, InitialAngle, DeltaTime, 3);
	OpenRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(OpenRotation);

	if(!AudioComponent) {return;}
	if(DoorSoundPlayed)
	{
		AudioComponent->Play();
		DoorSoundPlayed = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float CurrentMass = 0.f;

	// Find all Overlapping Actors
	TArray<AActor*> OverlappingActors;
	if(!PressurePlate) {return CurrentMass;}
	PressurePlate->GetOverlappingActors( OUT OverlappingActors);

	// Add up Actors Masses
	for (AActor* Actor : OverlappingActors)
	{
		CurrentMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return CurrentMass;
}

