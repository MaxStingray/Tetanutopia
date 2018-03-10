#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseWeapon.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Health.h"
#include "BaseItem.h"
#include "PlayerRobot.generated.h"

UCLASS()
class GENTEST_API APlayerRobot : public APawn, public IHealth
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = "_Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxHealth;
	UPROPERTY(Category = "_Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int CurrentHealth;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RobotMeshComponent;
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;	// The Camera Boom positions the attatched Camera Component above the Player

	// The equipment being used 
	UPROPERTY()
	UBaseWeapon* WeaponPrimary;
	UPROPERTY()
	UBaseWeapon* WeaponAlternate;
	UPROPERTY()
	UBaseItem* Item;

	// The offsets givin to the weapons position
	UPROPERTY(Category = "_Character|Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponPrimaryOffset;
	UPROPERTY(Category = "_Character|Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector WeaponAlternateOffset;

	// The particle effect that spawns at the thrusters
	// This can NOT be changed at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* Thruster;
	class UParticleSystemComponent* LeftThruster;
	class UParticleSystemComponent* RightThruster;

	// The sounds that the player will play
	// This can NOT be changed at runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* HurtSound;

	// The following input cannot be done through just a repeat as this had a major delay that is noticable during gameplay
	// Therefore, we have bools for when actions should be taking place. Which are toggled by events fired when input is pressed
	
	bool bIsFiringPrimary;
	bool bIsFiringAlternate;
	bool bIsUsingItem;

	// The following functions setup their specific niche when the playerrobot is constructed
	void InitialiseControls();	// These should relate to an existing axis or action button setup within Project Settings -> Input
	void InitialiseCamera();	// The options within the boom are how we affect the perspective
	void InitialisePlayerStats();	// MoveSpeed, AttackPower, etc.
	void InitialiseStaticMesh();	// The mesh we use, etc
	void InitialiseWeapons();	// Sets up the weapons and item as nullptr so we dont experiecne crashes
	void InitialiseSounds();	// The sounds the player calls in code

	// Calculates how much we should move depending on the axis and then does so
	void ApplyMovement(const float deltaSeconds);

	// Calculate the angle the robot should currently be looking at and then sets us to that angle
	void ApplyLook(const float deltaSeconds);

	// Each tick we fire this method and attempt to fire
	// This will fail is bIsFiringIsPrimary isnt true
	void FirePrimary();

	// Each tick we fire this method and attempt to fire
	// This will fail is bIsFiringIsAlternate isnt true
	void FireAlternate();

	// Each tick we fire this method and attempt to use the item
	// This will fail is bIsUsingItem isnt true
	void UseItem();

	// The following functions are used on the input events to toggle our variables as mentioned above 
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

	// When the player dies, this method is fire
	// Allowing us to play effects and stuff before destroying
	void OnDeath();

	// Keybindings that are used within this actor
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingMoveForward;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingMoveRight;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingLookForward;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingLookRight;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingPrimaryFire;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingAlternateFire;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingUseItem;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipPrimary;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipAlternate;
	UPROPERTY(Category = "_Character|Controls|Bindings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName BindingEquipItem;

	// The following provide options for how the character should behave
	UPROPERTY(Category = "_Character|Controls|Options", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bShootWithLook;	// Whether to use Primary Fire when a direction is selected
	UPROPERTY(Category = "_Character|Controls|Options", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bUseCameraForward;	// Whether forward should be in reltation to the camera, else the player
	UPROPERTY(Category = "_Character|Controls|Options", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bLookWithMove;		// Whether we should look in the direction we are moving

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	APlayerRobot();

	// The speed which the player traverses the environment
	UPROPERTY(Category = "_Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
	
	// The speed which the player traverses the environment
	UPROPERTY(Category = "_Character", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeedMult;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BluePrintCallable)
	UBaseWeapon* GetPrimaryWeapon() { return WeaponPrimary; };
	UFUNCTION(BluePrintCallable)
	UBaseWeapon* GetAlternateWeapon() { return WeaponAlternate; };
	UFUNCTION(BluePrintCallable)
	UBaseItem* GetItem() { return Item; }

	// Call this whenever you want to update the equipped weapon
	UFUNCTION(BluePrintCallable)
	void EquipWeaponPrimary(TSubclassOf<UBaseWeapon> weapon);
	UFUNCTION(BluePrintCallable)
	void EquipWeaponAlternate(TSubclassOf<UBaseWeapon> weapon);
	UFUNCTION(BluePrintCallable)
	void EquipItem(TSubclassOf<UBaseItem> weapon);
	
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

	UFUNCTION(BlueprintCallable)
	void SetInputActive(bool value);

	// Variables below this were added to quickly provide functionality.
	// Not the best programming practices, but it would take time to refactor
	bool bIsVulnerable;
	bool bIsMoving;

	// Whether the player is trying to pickup a primary / alternate weapon
	bool bPickingPrimaryWeapon;
	bool bPickingAlternateWeapon;
	bool bPickingUpItem;
};