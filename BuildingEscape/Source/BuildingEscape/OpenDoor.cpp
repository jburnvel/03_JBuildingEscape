// Copyrigth Jorge Burnes 2017

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
 
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
	Owner = GetOwner();	
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Poll the Trigger Volume
	if (!DoorIsOpen)
	{
		if (GetTotalMassOfActorsOnPlate() > TriggerMass) 
		{
			DoorIsOpen = true;
			OnOpen.Broadcast();			
		}
	}
	else 
	{
		if (GetTotalMassOfActorsOnPlate() < TriggerMass)
		{
			DoorIsOpen = false;
			OnClose.Broadcast();
		}
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their mass
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}

	return TotalMass;
}









//Funciones para recordar
//// Find the owning objet
//FString ObjectName = Owner->GetName();
//FQuat ObjectRot = Owner->GetTransform().GetRotation();//"X=algo, Y=algo etc..."; 	
//UE_LOG(LogTemp, Warning, TEXT("%s is Rotation  X=%f, Y=%f, Z=%f"), *ObjectName, ObjectRot.X, ObjectRot.Y, ObjectRot.Z);