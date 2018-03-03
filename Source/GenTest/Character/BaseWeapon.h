#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "BaseWeapon.generated.h"

UCLASS(Blueprintable)
class GENTEST_API UBaseWeapon : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	// Whether we can fire
	bool bCanFire;

	// After a period of time this is called to reset the bCanFire
	void ReEnableCanFire();

	// The handle for the timer until the next shot can be fired
	FTimerHandle TimerHandle_TimeUntilCanFire;

	// The sound that plays when a bullet is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* ShootSound;
	
	// The name of the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

	// Setup the Shoot Sound
	void InitialiseSounds();

	// Setup the Static Mesh
	void InitialiseStaticMesh();

	// Setup weapon stats
	void InitialiseWeaponStats();

	// The projectile that is fire
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABaseProjectile> ProjectileType;

	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ProjectileMaxSpread;

	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ProjectilesToSpawnOnFire;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UBaseWeapon();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Fires the weapon
	UFUNCTION(BlueprintCallable)
	void Fire();

	// Gets the string name of the weapon
	UFUNCTION(BlueprintCallable)
	FString GetWeaponName();

	// Sets the offset
	virtual void SetOffset(FVector offset);

	// The offset the projectile spawns from the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector ProjectileSpawnOffset;

	// The offset the projectile spawns from the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponPositionOffset;

	// How fast the weapon fires each shot
	UPROPERTY(Category = "Weapon|Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FireInterval;


	void* operator new  (std::size_t count);
};
