#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "Engine/StaticMeshActor.h"
#include "HealthCollectable.generated.h"

UCLASS()
class GENTEST_API AHealthCollectable : public AStaticMeshActor
{
	GENERATED_BODY()

private:
	class USphereComponent* CollisionSphere;

	UPROPERTY(Category = "Health", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int HealthValue;

public:
	AHealthCollectable();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
