#pragma once

#include "CoreMinimal.h"
#include "Character/Pickup.h"
#include "BaseItem.h"
#include "ItemPickup.generated.h"

UCLASS()
class GENTEST_API AItemPickup : public APickup
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<UBaseItem> Item;

protected:
	UFUNCTION()
	void WhileOverlap(AActor* OtherActor) override;

	virtual void OverlapEnds(AActor* OtherActor) override;

public:
	AItemPickup();

	UFUNCTION(BluePrintCallable)
	void SetItem(TSubclassOf<UBaseItem> newItem);
};
