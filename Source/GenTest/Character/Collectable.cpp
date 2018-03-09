#include "Collectable.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "PlayerRobot.h"
#include "Engine/CollisionProfile.h"

ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// A root component so the collectable cna be moved
	// RootComponent cannot be set at runtime
	UStaticMeshComponent* newRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = newRoot;

	// The collision sphere
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	CollisionSphere->InitSphereRadius(110.0f);
	CollisionSphere->SetupAttachment(RootComponent);

	CollisionSphere->SetCollisionProfileName("Pickup");

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::StartOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ACollectable::EndOverlap);

	// Set to nullptr asthis will be setup at runtime
	Item = nullptr;
	bOverlappingPlayer = false;
	OverlappingActor = nullptr;
	
	bPickupAble = true;
}

void ACollectable::ReverseHover()
{
	HoverSpeed = -HoverSpeed;

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle_HoverDirection, this, &ACollectable::ReverseHover, HoverTime);
}

void ACollectable::ApplyRotation()
{
	FRotator rotation = GetActorRotation();
	rotation.Yaw += RotationSpeed;
	SetActorRotation(rotation);
}

void ACollectable::ApplyHover()
{
	SetActorLocation(FVector(0, 0, HoverSpeed) + GetActorLocation(), false); //TODO: Maybe sweep
}

void ACollectable::EnablePickup()
{
	bPickupAble = true;
}

void ACollectable::StartOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayerRobot::StaticClass()))
	{
		bOverlappingPlayer = true;
		OverlappingActor = OtherActor;
	}
}

void ACollectable::EndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	// We check if the actor that ended was the player
	bOverlappingPlayer = OtherActor->IsA(APlayerRobot::StaticClass());
	if (bOverlappingPlayer)
	{
		// if it was, we need to set overlapping to false and also null pointer to the actor
		OverlappingActor = nullptr;
		bOverlappingPlayer = false;
	}
}

void ACollectable::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle_HoverDirection, this, &ACollectable::ReverseHover, HoverTime / 2); // we do half the hovertime initially because we are in the middle
	
	if (ItemType && !Item)
	{
		SetItem();
	}
	else
	{
		Destroy();
	}
}

void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyRotation();
	ApplyHover();

	if (bOverlappingPlayer && bPickupAble)
	{
		OnOverlap();
	}
}

void ACollectable::OnOverlap()
{
	if (OverlappingActor)
	{
		//APlayerRobot* player = Cast<APlayerRobot>(OverlappingActor);
		//if (player->bPickingPrimaryWeapon)
		//{
		//	TSubclassOf<UStaticMeshComponent> temp = player->WeaponPrimaryType;
		//	player->EquipWeaponPrimary(ItemType);

		//	ItemType = temp;
		//	SetItem();
		//	bPickupAble = false;

		//	UWorld* const World = GetWorld();
		//	World->GetTimerManager().SetTimer(TimerHandle_CollectCooldown, this, &ACollectable::EnablePickup, PickupCooldown);
		//}
		//else if (player->bPickingAlternateWeapon)
		//{
		//	TSubclassOf<UStaticMeshComponent> temp = player->WeaponAlternateType;
		//	player->EquipWeaponAlternate(ItemType);

		//	ItemType = temp;
		//	SetItem();
		//	bPickupAble = false;

		//	UWorld* const World = GetWorld();
		//	World->GetTimerManager().SetTimer(TimerHandle_CollectCooldown, this, &ACollectable::EnablePickup,  PickupCooldown);
		//}
		//else
		//{
		//	const FString message = FString("Press A for Primary, B for Alternate");
		//	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, message);
		//}
	}
}

void ACollectable::SetItem()
{
	if (Item)
	{
		Item->DestroyComponent();
		Item = nullptr;
	}

	if (ItemType)
	{
		Item = NewObject<UStaticMeshComponent>(this, ItemType);
		Item->SetupAttachment(RootComponent);
		Item->AttachTo(RootComponent);
		Item->RegisterComponentWithWorld(GetWorld());	// If we use the CreateObject method we must register with the world for the render to work
		Item->SetCollisionProfileName("Pickup");
	}
}

void ACollectable::SetItem(TSubclassOf<UStaticMeshComponent> item)
{
	ItemType = item;
	SetItem();
}

