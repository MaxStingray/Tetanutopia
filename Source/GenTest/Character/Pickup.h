#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class GENTEST_API APickup : public AActor
{
	GENERATED_BODY()
	
private:
	class USphereComponent* CollisionSphere;

	// Rotation Settings
	UPROPERTY(Category = "_Collectable|Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RotationSpeed; // How fast the item rotates

	// Hover Settings
	UPROPERTY(Category = "_Collectable|Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HoverSpeed; // How fast the item moves up and down
	UPROPERTY(Category = "_Collectable|Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float HoverTime;	// How long before changing direction
	FTimerHandle TimerHandle_HoverDirection;

	// Cooldown Settings
	UPROPERTY(Category = "_Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PickupCooldown;	// Time before you can pick up the item
	FTimerHandle TimerHandle_CollectCooldown;
	bool bPickupable;

	// Called by timer to reverse the hover direciton
	void ReverseHover();
	// Called by timer to enable the pickup ability of the item
	void EnablePickup();

	// Overlap Events
	UFUNCTION()
	void StartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// The actor we are currently overlapping
	AActor* OverlappingActor;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartPickupCooldown();

	UFUNCTION()
	virtual void WhileOverlap(AActor* OtherActor);

	virtual void OverlapEnds(AActor* OtherActor) {};
public:	
	APickup();
	virtual void Tick(float DeltaTime) override;
};
