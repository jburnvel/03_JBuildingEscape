// Copyrigth Jorge Burnes 2017

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
 

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

	//Find the owning Actor
	Owner = GetOwner();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	// Find the owning objet
	FString ObjectName = Owner->GetName();
	FQuat ObjectRot = Owner->GetTransform().GetRotation();//"X=algo, Y=algo etc..."; 	
	UE_LOG(LogTemp, Warning, TEXT("%s is Rotation  X=%f, Y=%f, Z=%f"), *ObjectName, ObjectRot.X, ObjectRot.Y, ObjectRot.Z);
}

// Open Door by angle set in OpenAngle
void UOpenDoor::OpenDoor()
{ 
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

// Close Door by angle set in OpenAngle
void UOpenDoor::CloseDoor()
{ 
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	//Check if it's time to colse the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	} 
}