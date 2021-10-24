// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Actor.h"
#include "OpenDoor.h"

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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentAngle = FMath::FInterpTo(CurrentAngle, OpenDoorAngle, DeltaTime, 1);
	OpenRotation.Yaw = CurrentAngle;
	GetOwner()->SetActorRotation(OpenRotation);
}

