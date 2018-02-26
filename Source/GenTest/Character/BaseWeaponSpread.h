#pragma once

// Behaviour has been moved to the BaseWweapon, do not use this class

#include "CoreMinimal.h"
#include "Character/BaseWeapon.h"
#include "BaseWeaponSpread.generated.h"

UCLASS()
class GENTEST_API UBaseWeaponSpread : public UBaseWeapon
{
	GENERATED_BODY()
	
//protected:
//	// The spread offset the projectile spawns from the weapon
//	// Plus or minus this vector
//	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
//	int ProjectileMaxSpread;
//public:
//	UBaseWeaponSpread();
//	// Fires the weapon with spread
//	virtual void Fire() override;
};
