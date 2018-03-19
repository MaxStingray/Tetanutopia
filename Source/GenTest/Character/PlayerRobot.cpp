#include "PlayerRobot.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/Particles/ParticleSystem.h"

APlayerRobot::APlayerRobot()
{
	PrimaryActorTick.bCanEverTick = true;	// Enables the Tick event from triggering every frame

	InitialiseStaticMesh();
	InitialisePlayerStats();
	InitialiseControls();
	InitialiseCamera();
	InitialiseWeapons();
	InitialiseSounds();
}

void APlayerRobot::InitialiseSounds()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> shootCue(TEXT("/Game/Audio/Sounds/Equip_Weapon_Cue"));
	EquipSound = shootCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> hurtCue(TEXT("/Game/Audio/Sounds/Hit_Short_Cue"));
	HurtSound = hurtCue.Object;
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
	bUseCameraForward = false;
	bLookWithMove = false;
	bInvertControls = false;

	bDisplayItemPickup = false;
	bDisplayWeaponPickup = false;
	PickupContext = "";

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
	CameraBoom->TargetArmLength = 600.0f; // The distance from the Robot that the camera is positioned
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f); // The angle of the camera

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate with the Camera Boom

	// Sets Up Camera Lag
	// This means the player will not always be at the center, feels more dynamic
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagMaxDistance = 100;	// The furthest additional distance the camera will be from the player
	CameraBoom->CameraLagSpeed = 5;	// The speed of the camera at catching the minimum distance from the player
}

void APlayerRobot::InitialisePlayerStats()
{
	bIsMoving = false;
	MoveSpeed = 1000.0f;
	MoveSpeedMult = 1;

	bIsVulnerable = true;

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
	WeaponPrimaryOffset = FVector(0, 55, 35);
	WeaponAlternateOffset = FVector(0, -55, 35);

	WeaponPrimary = nullptr;
	WeaponAlternate = nullptr;
	Item = nullptr;
}

void APlayerRobot::ApplyMovement(const float deltaSeconds)
{
	const float MoveForward = !bInvertControls ? GetInputAxisValue(BindingMoveForward) : GetInputAxisValue(BindingMoveForward) * -1;
	const float MoveRight = !bInvertControls ? GetInputAxisValue(BindingMoveRight) : GetInputAxisValue(BindingMoveRight) * -1;

	const FVector MoveDirection = FVector(MoveForward, MoveRight, 0.f).GetClampedToMaxSize(1.0f);	// Clamp to ensure we cant travel faster diagonally
	FVector movement = MoveDirection * MoveSpeed * MoveSpeedMult * deltaSeconds;

	if (bUseCameraForward)
	{
		// Apply Correction to movement so that the direction is relative to the camera, not the player model
		FRotator YawOfCamera = FRotator(0.f, CameraComponent->GetComponentRotation().Yaw, 0.f);
		movement = YawOfCamera.RotateVector(movement);
	}

	if (movement.SizeSquared() > FLT_EPSILON)
	{
		bIsMoving = true;

		FHitResult Hit(1.f);
		AddActorWorldOffset(movement, true, &Hit);
		//RobotMeshComponent->AddImpulse(movement * 1000 * deltaSeconds);

		const float LookForward = !bInvertControls ? GetInputAxisValue(BindingLookForward) : GetInputAxisValue(BindingLookForward) * -1;
		const float LookRight = !bInvertControls ? GetInputAxisValue(BindingLookRight) : GetInputAxisValue(BindingLookRight) * -1;

		const FVector LookDirection = FVector(LookForward, LookRight, 0.f);
		if (bLookWithMove && LookDirection.SizeSquared() < FLT_EPSILON)
		{
			FRotator MoveRotation = GetActorRotation();
			// Make the player look in the direction of movement if not looking actively
			MoveRotation.Yaw = FMath::Lerp(MoveRotation.Yaw, MoveDirection.Rotation().Yaw, 0.5f);
			SetActorRotation(MoveRotation);
		}

		// TODO: Tilting

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
		bIsMoving = false;

		//	TODO: Stop Tilt
	}
}

void APlayerRobot::ApplyLook(const float deltaSeconds)
{
	const float LookForward = !bInvertControls ? GetInputAxisValue(BindingLookForward) : GetInputAxisValue(BindingLookForward) * -1;
	const float LookRight = !bInvertControls ? GetInputAxisValue(BindingLookRight) : GetInputAxisValue(BindingLookRight) * -1;

	const FVector LookDirection = FVector(LookForward, LookRight, 0.f);

	if (LookDirection.SizeSquared() > FLT_EPSILON)
	{
		FRotator lookRotation = GetActorRotation();
		float yawTemp = LookDirection.Rotation().Yaw;

		if (bUseCameraForward)
		{
			yawTemp += CameraComponent->GetComponentRotation().Yaw; // Fix the rotation to be based on the camera, rather than the player
		}

		lookRotation.Yaw = FMath::Lerp(lookRotation.Yaw, yawTemp, 0.5f);

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
	if (bIsFiringPrimary && WeaponPrimary)
	{
		WeaponPrimary->Fire();
	}
}

void APlayerRobot::FireAlternate()
{
	if (bIsFiringAlternate && WeaponAlternate)
	{
		WeaponAlternate->Fire();
	}
}

void APlayerRobot::UseItem()
{
	if (bIsUsingItem && Item)
	{
		Item->Use();
	}
}

void APlayerRobot::OnDeath()
{
	// TODO: Other stuff
	UE_LOG(LogTemp, Warning, TEXT("**** PLAYER HAS DIED ****"));
	Destroy(); 
}

void APlayerRobot::MakeInvulnerable()
{
	const float timeInvulnerable = 1.0f;
	bIsVulnerable = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_TimeUntilVulnerable, this, &APlayerRobot::MakeVulnerable, timeInvulnerable);
	UE_LOG(LogTemp, Warning, TEXT("Player is immune"));
}

void APlayerRobot::MakeVulnerable()
{
	bIsVulnerable = true;
	UE_LOG(LogTemp, Warning, TEXT("Player is no longer immune"));
}

void APlayerRobot::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	// Required by Unreal

	if (Thruster && !LeftThruster)
	{
		LeftThruster = UGameplayStatics::SpawnEmitterAttached(Thruster, RootComponent, NAME_None, FVector(-60.0f,-10.0f,5.0f));
		FinishAndRegisterComponent(LeftThruster);
	}
	if (Thruster && !RightThruster)
	{
		RightThruster = UGameplayStatics::SpawnEmitterAttached(Thruster, RootComponent, NAME_None, FVector(-60.0f,10.0f,5.0f));
		FinishAndRegisterComponent(RightThruster);
	}

	ApplyMovement(DeltaTime);
	ApplyLook(DeltaTime);
	FirePrimary();
	FireAlternate();
	UseItem();
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

void APlayerRobot::EquipWeaponPrimary(TSubclassOf<UBaseWeapon> weapon)
{
	if (WeaponPrimary) { WeaponPrimary->DestroyComponent(); };
	if (weapon)
	{
		WeaponPrimaryType = weapon;

		WeaponPrimary = NewObject<UBaseWeapon>(this, weapon);
		WeaponPrimary->SetOffset(WeaponPrimaryOffset);
		WeaponPrimary->AttachTo(RootComponent);
		WeaponPrimary->SetupAttachment(RootComponent);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::EquipWeaponAlternate(TSubclassOf<UBaseWeapon> weapon)
{
	if (WeaponAlternate) { WeaponAlternate->DestroyComponent(); };
	if (weapon)
	{
		WeaponAlternateType = weapon;

		WeaponAlternate = NewObject<UBaseWeapon>(this, weapon);
		WeaponAlternate->SetOffset(WeaponAlternateOffset);
		WeaponAlternate->AttachTo(RootComponent);
		WeaponAlternate->SetupAttachment(RootComponent);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::EquipItem(TSubclassOf<UBaseItem> item)
{
	if (Item) { Item->DestroyComponent(); };
	if (item)
	{
		ItemType = item;

		Item = NewObject<UBaseItem>(this, item);
		FinishAndRegisterComponent(Item);

		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation(), GetActorRotation());
	}
}

void APlayerRobot::TakeDamage(int amount)
{
	if(amount > 0 && bIsVulnerable)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation(), GetActorRotation());

		CurrentHealth -= amount;

		if (CurrentHealth <= 0)
		{
			OnDeath();
		}

		MakeInvulnerable();
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

void APlayerRobot::SetInputActive(bool value)
{
	if (value)
	{
		EnableInput(Cast<APlayerController>(this));
	}
	else
	{
		DisableInput(Cast<APlayerController>(this));

		// We have to manually false out bools or else the input ends up locking
		bIsUsingItem = false;
		bIsFiringAlternate = false;
		bIsFiringPrimary = false;
	}
}
