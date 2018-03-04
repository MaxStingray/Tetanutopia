#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseWeapon.h"
#include "Collectable.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Health.h"
#include "PlayerRobot.generated.h"

UCLASS()
class GENTEST_API APlayerRobot : public APawn, public IHealth
{
	GENERATED_BODY()

private:
	// The max health the player can have at a single time
	UPROPERTY(Category = "Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxHealth;

	// The current health the player has. When the player dies (reaches 0) on death will be called.
	UPROPERTY(Category = "Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int CurrentHealth;

	// The visible representation of the player
	// This is the actual robot that will physically move around the environment
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RobotMeshComponent;

	// The attached camera for this player
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	// The Camera Boom positions the attatched Camera Component above the Player
	// The options within the boom are how we affect the perspective
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// The actual reference to the weapon
	UBaseWeapon* WeaponPrimary;

	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponPrimaryOffset;

	// The muzzle flash particle effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ThrusterTemplate;

	class UParticleSystemComponent* LeftThruster;
	class UParticleSystemComponent* RightThruster;

	// The actual alternate weapon equipped
	UBaseWeapon* WeaponAlternate;

	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponAlternateOffset;

	// The active item equipped
	UBaseWeapon* ItemActive;

	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector ItemOffset;

	// The sound that plays when you equip something
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;

	// The sound that plays when a you take damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* HurtSound;

	// The following input cannot be done through just a repeat as this had a major delay that is noticable during gameplay
	// Whether the primary fire button is being pressed
	bool bIsFiringPrimary;

	// Whether the secondary fire button is being pressed
	bool bIsFiringAlternate;

	// Whether the item use button is being pressed
	bool bIsUsingItem;

	// Initialises the Binding strings
	// These should relate to an existing axis or action button setup within Project Settings -> Input
	void InitialiseControls();

	// Initialises the Camera and Camera Boom
	// The options within the boom are how we affect the perspective
	void InitialiseCamera();

	// Initialises the Player Stats
	// MoveSpeed, AttackPower, etc.
	void InitialisePlayerStats();

	// Sets up the Mesh by loading it from file
	// Additionally sets up the Static Mesh Component
	void InitialiseStaticMesh();

	// Sets up the Weapons
	// Provides some default starting weapons
	void InitialiseWeapons();

	// Calculates how much we should move depending on the axis and then does so
	void ApplyMovement(const float deltaSeconds);

	// Calculate the angle the robot should currently be looking at and then sets us to that angle
	void ApplyLook(const float deltaSeconds);

	// The Primary Fire Action
	void FirePrimary();

	// The Alternate Fire Action
	void FireAlternate();

	// The Action to use the Active Item
	void UseActiveItem();

	void StartFiringPrimary() { bIsFiringPrimary = true; };
	void StopFiringPrimary() { bIsFiringPrimary = false; };

	void StartFiringAlternate() { bIsFiringAlternate = true; };
	void StopFiringAlternate() { bIsFiringAlternate = false; };

	void StartUsingItem() { bIsUsingItem = true; };
	void StopUsingItem() { bIsUsingItem = false; };

	void StartPickingPrimary() { bPickingPrimaryWeapon = true; bPickingAlternateWeapon = false; };
	void StopPickingPrimary() { bPickingPrimaryWeapon = false; };
	void StartPickingAlternate() { bPickingAlternateWeapon = true; bPickingPrimaryWeapon = false; };
	void StopPickingAlternate() { bPickingAlternateWeapon = false; };

	void StartPickingItem() { bPickingUpItem = true; };
	void StopPickingItem() { bPickingUpItem = false; };

	// This function is called when the players health reaches 0
	UFUNCTION(BlueprintCallable)
	void OnDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	APlayerRobot();

	// The type of Primary Weapon equipped
	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseWeapon> WeaponPrimaryType;

	// The type of Alternate Weapon equiped
	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseWeapon> WeaponAlternateType;

	// The type of item type equiped
	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseWeapon> ItemType;

	// The speed which the player traverses the environment
	UPROPERTY(Category = "Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// Keybindings that are used within this actor
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingMoveForward;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingMoveRight;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingLookForward;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingLookRight;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingPrimaryFire;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingAlternateFire;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingUseItem;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipPrimary;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipAlternate;
	UPROPERTY(Category = "Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipItem;

	// Whether to use Primary Fire when a direction is selected
	UPROPERTY(Category = "Controls|Options", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bShootWithLook;

	// Whether forward should be in reltation to the camera, else the player
	UPROPERTY(Category = "Controls|Options", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bUseCameraForward;

	// Whether the player is trying to pickup a primary / alternate weapon
	bool bPickingPrimaryWeapon;
	bool bPickingAlternateWeapon;
	bool bPickingUpItem;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns the Robot mesh component of this Actor
	FORCEINLINE class UStaticMeshComponent* GetRobotMeshComponent() const { return RobotMeshComponent; }

	// Returns the Camera component of this Actor
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	// Returns the Camera Boom component of this Actor
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BluePrintCallable)
	UBaseWeapon* GetPrimaryWeapon() { return WeaponPrimary; };

	UFUNCTION(BluePrintCallable)
	UBaseWeapon* GetAlternateWeapon() { return WeaponAlternate; };

	// Call this whenever you want to update the equipped weapon
	void EquipWeaponPrimary();
	void EquipWeaponPrimary(TSubclassOf<UStaticMeshComponent> weapon);
	void EquipWeaponAlternate();
	void EquipWeaponAlternate(TSubclassOf<UStaticMeshComponent> weapon);
	void EquipItemActive();
	void EquipItemActive(TSubclassOf<UStaticMeshComponent> weapon);
	
	// Unequip Method
	void UnequipWeaponPrimary();
	void UnequipWeaponAlternate();
	void UnequipItemActive();

	// Method for taking damage
	UFUNCTION(BlueprintCallable)
	void TakeDamage(int amount);
	
	// Method for healing health
	// In theory you could use negative take damage, but for the sake of readability both are included
	UFUNCTION(BlueprintCallable)
	void Heal(int amount);

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth();
};