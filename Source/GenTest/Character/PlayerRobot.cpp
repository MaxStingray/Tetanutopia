#include "PlayerRobot.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Collectable.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"

APlayerRobot::APlayerRobot()
{
	PrimaryActorTick.bCanEverTick = true;	// Enables the Tick event from triggering every frame

	InitialiseStaticMesh();
	InitialisePlayerStats();
	InitialiseControls();
	InitialiseCamera();
	InitialiseWeapons();

	// Setup SOunds
	static ConstructorHelpers::FObjectFinder<USoundCue> shootCue(TEXT("/Game/Audio/pickup_cue"));
	EquipSound = shootCue.Object;
}

void APlayerRobot::InitialiseControls()
{
	BindingMoveForward = "MoveForward";
	BindingMoveRight = "MoveRight";
	BindingLookForward = "LookForward";
	BindingLookRight = "LookRight";
	BindingPrimaryFire = "PrimaryFire";
	BindingAlternateFire = "AlternateFire";
	BindingUseItem = "UseItem";
	BindingEquipPrimary = "EquipPrimary";
	BindingEquipAlternate = "EquipAlternate";
	BindingEquipItem = "EquipItem";

	bShootWithLook = false;
	bUseCameraForward = true;

	bIsFiringPrimary = false;
	bIsFiringAlternate = false;
	bPickingPrimaryWeapon = false;
	bPickingAlternateWeapon = false;
	bPickingUpItem = false;
	bIsUsingItem = false;
}

void APlayerRobot::InitialiseCamera()
{
	// Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Prevents the arm from rotating as the robot rotates
	CameraBoom->bDoCollisionTest = false; // Prevents the camera from pulling when it colliders with terrain
	CameraBoom->TargetArmLength = 1600.f; // The distance from the Robot that the camera is positioned
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f); // The angle of the camera

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate with the Camera Boom

	// Sets Up Camera Lag
	// This means the player will not always be at the center, feels more dynamic
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagMaxDistance = -50;	// The furthest additional distance the camera will be from the player
	CameraBoom->CameraLagSpeed = 5;	// The speed of the camera at catching the minimum distance from the player
}

void APlayerRobot::InitialisePlayerStats()
{
	MoveSpeed = 1000.0f;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
}

void APlayerRobot::InitialiseStaticMesh()
{
	// Setup the Static Mesh Component
	RobotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobotMesh"));
	RootComponent = RobotMeshComponent;
	RobotMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RobotMeshComponent->bGenerateOverlapEvents = true;

	bGenerateOverlapEventsDuringLevelStreaming = true;

	// Find the mesh and apply it to the Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RobotMesh(TEXT("/Game/Geometry/Meshes/Character"));
	RobotMeshComponent->SetStaticMesh(RobotMesh.Object);
}

void APlayerRobot::InitialiseWeapons()
{
	WeaponPrimaryOffset = FVector(0, 100, 0);
	WeaponAlternateOffset = FVector(0, -100, 0);
	ItemOffset = FVector(0, 0, 0);

	WeaponPrimary = nullptr;
	ItemActive = nullptr;
	WeaponAlternate = nullptr;

	// Equip the weapons of the correct type
	// IMPORTANT: For this to work correctly it cant be done in the constructor! instead they are called in beingplayer
	//EquipWeaponPrimary();
	//EquipWeaponAlternate();
}

void APlayerRobot::ApplyMovement(const float deltaSeconds)
{
	const float MoveForward = GetInputAxisValue(BindingMoveForward);
	const float MoveRight = GetInputAxisValue(BindingMoveRight);

	const FVector MoveDirection = FVector(MoveForward, MoveRight, 0.f).GetClampedToMaxSize(1.0f);	// Clamp to ensure we cant travel faster diagonally
	FVector movement = MoveDirection * MoveSpeed * deltaSeconds;


	if (bUseCameraForward)
	{
		// Apply Correction to movement so that the direction is relative to the camera, not the player model
		FRotator YawOfCamera = FRotator(0.f, CameraComponent->GetComponentRotation().Yaw, 0.f);
		movement = YawOfCamera.RotateVector(movement);
	}

	if (movement.SizeSquared() > FLT_EPSILON)
	{
		FHitResult Hit(1.f);
		AddActorWorldOffset(movement, true, &Hit);

		// Make the player look in the direction of movement
		const FRotator MoveRotation = MoveDirection.Rotation().GetNormalized();
		SetActorRotation(MoveRotation);
		
		// Tilt the player because we are moving
		FRotator current = GetActorRotation();
		current.Pitch = -10;
		SetActorRotation(current);

		// Collision Handling
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(movement, Normal2D) * (1.f - Hit.Time);
			AddActorWorldOffset(Deflection, true);
		}
	}
	else
	{
		// Stop tilting the player
		FRotator current = GetActorRotation();
		current.Pitch = 0;
		SetActorRotation(current);
	}
}

void APlayerRobot::ApplyLook(const float deltaSeconds)
{
	const float LookForward = GetInputAxisValue(BindingLookForward);
	const float LookRight = GetInputAxisValue(BindingLookRight);

	const FVector LookDirection = FVector(LookForward, LookRight, 0.f);

	if (LookDirection.SizeSquared() > FLT_EPSILON)
	{
		FRotator lookRotation = LookDirection.Rotation(); // Get the rotation from the joystick

		if (bUseCameraForward)
		{
			lookRotation.Yaw += CameraComponent->GetComponentRotation().Yaw; // Fix the rotation to be based on the camera, rather than the player
		}

		if (bShootWithLook)
		{
			bIsFiringPrimary = true;
		}

		SetActorRotation(lookRotation);
	}
	else
	{
		if (bShootWithLook)
		{
			bIsFiringPrimary = false;
		}
	}
}

void APlayerRobot::FirePrimary()
{
	if (bIsFiringPrimary)
	{
		if (WeaponPrimary)
		{
			WeaponPrimary->Fire();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("No Primary Weapon"));
		}
	}
}

void APlayerRobot::FireAlternate()
{
	if (bIsFiringAlternate)
	{
		if (WeaponAlternate)
		{
			WeaponAlternate->Fire();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("No Alternate Weapon"));
		}
	}
}

void APlayerRobot::UseActiveItem()
{
	if (bIsUsingItem)
	{
		if(ItemActive)
		{
			ItemActive->Fire();
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("No Item"));
		}
	}
}

void APlayerRobot::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Player had died!"));
	Destroy(); // TODO: Other stuff
}

void APlayerRobot::BeginPlay()
{
	Super::BeginPlay();	// Required by Unreal

	// Equip the correct weapons as the weapontype says to.
	EquipItemActive();
	EquipWeaponPrimary();
	EquipWeaponAlternate();
}

void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	// Required by Unreal

	ApplyMovement(DeltaTime);
	ApplyLook(DeltaTime);
	FirePrimary();
	FireAlternate();
	UseActiveItem();
}

void APlayerRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	// Required by Unreal

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis(BindingMoveForward);
	PlayerInputComponent->BindAxis(BindingMoveRight);
	PlayerInputComponent->BindAxis(BindingLookForward);
	PlayerInputComponent->BindAxis(BindingLookRight);

	PlayerInputComponent->BindAction(BindingPrimaryFire, EInputEvent::IE_Pressed, this, &APlayerRobot::StartFiringPrimary);
	PlayerInputComponent->BindAction(BindingPrimaryFire, EInputEvent::IE_Released, this, &APlayerRobot::StopFiringPrimary);

	PlayerInputComponent->BindAction(BindingAlternateFire, EInputEvent::IE_Pressed, this, &APlayerRobot::StartFiringAlternate);
	PlayerInputComponent->BindAction(BindingAlternateFire, EInputEvent::IE_Released, this, &APlayerRobot::StopFiringAlternate);

	PlayerInputComponent->BindAction(BindingUseItem, EInputEvent::IE_Pressed, this, &APlayerRobot::StartUsingItem);
	PlayerInputComponent->BindAction(BindingUseItem, EInputEvent::IE_Released, this, &APlayerRobot::StopUsingItem);

	PlayerInputComponent->BindAction(BindingEquipPrimary, EInputEvent::IE_Pressed, this, &APlayerRobot::StartPickingPrimary);
	PlayerInputComponent->BindAction(BindingEquipPrimary, EInputEvent::IE_Released, this, &APlayerRobot::StopPickingPrimary);

	PlayerInputComponent->BindAction(BindingEquipAlternate, EInputEvent::IE_Pressed, this, &APlayerRobot::StartPickingAlternate);
	PlayerInputComponent->BindAction(BindingEquipAlternate, EInputEvent::IE_Released, this, &APlayerRobot::StopPickingAlternate);

	PlayerInputComponent->BindAction(BindingEquipItem, EInputEvent::IE_Pressed, this, &APlayerRobot::StartPickingItem);
	PlayerInputComponent->BindAction(BindingEquipItem, EInputEvent::IE_Released, this, &APlayerRobot::StopPickingItem);
}

void APlayerRobot::EquipWeaponPrimary()
{
	if (WeaponPrimaryType)
	{
		WeaponPrimary = NewObject<UBaseWeapon>(this, WeaponPrimaryType);
		WeaponPrimary->SetupAttachment(RootComponent);
		WeaponPrimary->SetOffset(WeaponPrimaryOffset);
		WeaponPrimary->AttachTo(RootComponent);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::EquipWeaponPrimary(TSubclassOf<UStaticMeshComponent> weapon)
{
	UnequipWeaponPrimary();
	WeaponPrimaryType = weapon;
	EquipWeaponPrimary();
}

void APlayerRobot::EquipWeaponAlternate()
{
	if (WeaponAlternateType)
	{
		WeaponAlternate = NewObject<UBaseWeapon>(this, WeaponAlternateType);
		WeaponAlternate->SetupAttachment(RootComponent);
		WeaponAlternate->SetOffset(WeaponAlternateOffset);
		WeaponAlternate->AttachTo(RootComponent);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::EquipWeaponAlternate(TSubclassOf<UStaticMeshComponent> weapon)
{
	UnequipWeaponAlternate();
	WeaponAlternateType = weapon;
	EquipWeaponAlternate();
}

void APlayerRobot::EquipItemActive()
{
	if(ItemType)
	{
		ItemActive = NewObject<UBaseWeapon>(this, ItemType);
		ItemActive->SetupAttachment(RootComponent);
		ItemActive->SetOffset(ItemOffset);
		ItemActive->AttachTo(RootComponent);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::EquipItemActive(TSubclassOf<UStaticMeshComponent> weapon)
{
	UnequipItemActive();
	ItemType = weapon;
	EquipItemActive();
}

void APlayerRobot::UnequipWeaponPrimary()
{
	if (WeaponPrimary)
	{
		// Delete the current component
		WeaponPrimary->DestroyComponent();
	}
}

void APlayerRobot::UnequipWeaponAlternate()
{
	if (WeaponAlternate)
	{
		// Delete the current component
		WeaponAlternate->DestroyComponent();
	}
}

void APlayerRobot::UnequipItemActive()
{
	if(ItemActive)
	{
		ItemActive->DestroyComponent();
	}
}

void APlayerRobot::TakeDamage(int amount)
{
	if(amount > 0)
	{
		CurrentHealth -= amount;

		if (CurrentHealth <= 0)
		{
			OnDeath();
		}
	}
}

void APlayerRobot::Heal(int amount)
{
	if (amount > 0)
	{
		CurrentHealth += amount;

		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
}

int APlayerRobot::GetMaxHealth()
{
	return MaxHealth;
}

int APlayerRobot::GetCurrentHealth()
{
	return CurrentHealth;
}
