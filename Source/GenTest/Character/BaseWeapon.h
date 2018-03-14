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

	//whether the next burst is ready (if burst fire enabled)
	bool bCanBurstFire;
	// After a period of time this is called to reset the bCanFire
	void ReEnableCanFire();

	int shotCount;

	//reset the bCanBurstFire
	void ReEnableBurstFire();


	// The handle for the timer until the next shot can be fired
	FTimerHandle TimerHandle_TimeUntilCanFire;

	FTimerHandle TimerHandle_TimeUntilBurstFire;

	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSpreadImpactsOffset;

	// The sound that plays when a bullet is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* ShootSound;
	
	// The name of the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString WeaponName;

	// The muzzle flash particle effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlashTemplate;

	class UParticleSystemComponent* MuzzleFlash;

	// Setup the Shoot Sound
	void InitialiseSounds();

	// Setup the Static Mesh
	void InitialiseStaticMesh();

	// Setup weapon stats
	void InitialiseWeaponStats();

	//randomise offset for rapid-fire weapons
	void RandomiseOffset(FVector offset);

	// The projectile that is fire
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> ProjectileType;

	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ProjectileMaxSpread;

	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool isBurstWeapon;

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

	UPROPERTY(EditAnywhere)
	FVector initialProjectileOffset;

	// Sets the offset
	virtual void SetOffset(FVector offset);

	// The offset the projectile spawns from the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector ProjectileSpawnOffset;

	// The offset the projectile spawns from the weapon
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponPositionOffset;

	//randomise the offset somewhat. For rapid-fire weapons. This will randomise from the given offset + offset x 2
	UPROPERTY(Category = "Weapon|Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool randomiseOffset;

	// How fast the weapon fires each shot
	UPROPERTY(Category = "Weapon|Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FireInterval;

	//how frequently a weapon can fire a burst, if set to 0 burst mode is disabled. Should always be longer than FireInterval
	UPROPERTY(Category = "Weapon|Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float burstFireInterval;

	//number of shots per burst
	UPROPERTY(Category = "Weapon|Stats", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int burstSize;

};
