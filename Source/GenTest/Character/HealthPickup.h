#pragma once

#include "CoreMinimal.h"
#include "Character/Pickup.h"
#include "HealthPickup.generated.h"

UCLASS()
class GENTEST_API AHealthPickup : public APickup
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = "_Collectable", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int HealthToGrant;

protected:
	UFUNCTION()
	void WhileOverlap(AActor* OtherActor) override;

public:
	AHealthPickup();
};
