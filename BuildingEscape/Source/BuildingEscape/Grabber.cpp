// Copyrigth Jorge Burnes 2017

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsComponent()
{	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physic Handle is not added in blueprint please add Physic Handle in %s"), *(GetOwner()->GetName()))
	}
}

/// Look for attached Input Component
void UGrabber::SetupInputComponent()
{	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component is not added in blueprint please add Input Component in %s"), *(GetOwner()->GetName()))
	}
}


void UGrabber::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();
	
	/// If we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bone needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
			);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();	 
}

/// Line trace (AKA ray-cast) out to reach distance and return HIT result
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
		 
	/// Line trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());// Setup query parameters
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

/// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 
	if (!PhysicsHandle)	{ return; }
	// If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
		//PhysicsHandle->SetTargetRotation(PlayerViewPointRotation*-1);
	}
}

/// Get player view point
FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

/// Get player view point
FVector UGrabber::GetReachLineEnd() const
{	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

//funciones para recordar
/// Draw a red trace in the world Efector 
//UKismetSystemLibrary::DrawDebugLine(GetWorld(),PlayerViewPointLocation, LineTraceEnd, FLinearColor(255,0,0), 0.f, 0.f);
/// See what we hit
//AActor* ActorHit = HitResult.GetActor();
//if (ActorHit)
//{
//UE_LOG(LogTemp, Warning, TEXT("HIT With: %s"), (*ActorHit->GetName()));
//} 

//
///// Get player view point this tick
//FVector PlayerViewPointLocation;
//FRotator PlayerViewPointRotation;
//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
//	OUT PlayerViewPointLocation,
//	OUT PlayerViewPointRotation
//);
//
//FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
