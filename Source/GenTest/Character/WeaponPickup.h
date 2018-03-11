#pragma once

#include "CoreMinimal.h"
#include "Character/Pickup.h"
#include "BaseWeapon.h"
#include "WeaponPickup.generated.h"

UCLASS()
class GENTEST_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<UBaseWeapon> WeaponType;
	UPROPERTY()
	UBaseWeapon *Weapon;

protected:
	UFUNCTION()
	void WhileOverlap(AActor* OtherActor) override;

public:
	AWeaponPickup();

	UFUNCTION(BluePrintCallable)
	void SetWeapon(TSubclassOf<UBaseWeapon> newWeapon);
};
