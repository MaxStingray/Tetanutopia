#include "Pickup.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "PlayerRobot.h"

APickup::APickup()
{
	// Setup vars
	PrimaryActorTick.bCanEverTick = true;

	RotationSpeed = 0.5f;
	PickupCooldown = 1.0f;
	bPickupable = false;
	HoverSpeed = 0.5f;
	HoverTime = 2.0f;

	OverlappingActor = nullptr;

	// Setup the Root Component
	UStaticMeshComponent* newRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = newRoot;

	// Setup the Collision Sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	CollisionSphere->InitSphereRadius(110.0f);
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionProfileName("Pickup");
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::StartOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::EndOverlap);
}

void APickup::ReverseHover()
{
	HoverSpeed = -1.0f * HoverSpeed;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HoverDirection, this, &APickup::ReverseHover, HoverTime);
}

void APickup::EnablePickup()
{
	bPickupable = true;
}

void APickup::StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (bPickupable)
	{
		if (OtherActor->IsA(APlayerRobot::StaticClass()))
		{
			OverlappingActor = OtherActor;
		}
	}
}

void APickup::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OverlappingActor == OtherActor)
	{
		OverlapEnds(OverlappingActor);
		OverlappingActor = nullptr;
	}
}

void APickup::WhileOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Base Pickup: Overlapping Player!"));
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	StartPickupCooldown();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HoverDirection, this, &APickup::ReverseHover, HoverTime / 2);
}

void APickup::StartPickupCooldown()
{
	OverlappingActor = nullptr;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CollectCooldown, this, &APickup::EnablePickup, PickupCooldown);
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate in place
	SetActorRotation(GetActorRotation() + FRotator(0.0f, RotationSpeed, 0.0f));
	// Hover up and down
	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, HoverSpeed));

	if (OverlappingActor != nullptr)
	{
		WhileOverlap(OverlappingActor);
	}
}

