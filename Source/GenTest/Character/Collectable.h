#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Collectable.generated.h"

UCLASS()
class GENTEST_API ACollectable : public AActor
{
	GENERATED_BODY()

protected:
	// The collision sphere (using the mesh from the weapon was an exercise in frustration)
	class USphereComponent* CollisionSphere;

	// How fast the item rotates on the spot
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 0.5f;

	// How fast the item moves up + down
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HoverSpeed = 0.5f;

	// The amount of time before changing direction
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HoverTime = 2.0f;

	// Timer which controls the hover direction
	FTimerHandle TimerHandle_HoverDirection;

	// The amount of time before the item cna be picked up after being picked up previous
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PickupCooldown = 1.0f;

	// Timer that handles the cooldown between picking this collectable up after it recently has already been
	FTimerHandle TimerHandle_CollectCooldown;

	// Reverses the HoverSpeed therefore changing the direction
	void ReverseHover();

	// Applys the Rotation
	void ApplyRotation();

	// Applys the Hover
	void ApplyHover();
	
	// Sets the item as pickupable
	void EnablePickup();

	// The actual item
	UStaticMeshComponent* Item;

	// Whether the item can be picked up
	bool bPickupAble;

	// Overlap Event
	bool bOverlappingPlayer;
	AActor* OverlappingActor;
	UFUNCTION()
	void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACollectable();

	// The type of the item
	UPROPERTY(Category = "Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStaticMeshComponent> ItemType;

	UFUNCTION()
	virtual void OnOverlap();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Update item
	void SetItem();
	void SetItem(TSubclassOf<UStaticMeshComponent> item);
};
