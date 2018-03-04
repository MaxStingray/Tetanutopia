#include "BaseWeapon.h"
#include "BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/Particles/ParticleSystem.h"
#include "TimerManager.h"
#include <string>
#include "Kismet/GameplayStatics.h"

UBaseWeapon::UBaseWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponentWithWorld(GetWorld());	// If we use the CreateObject method we must register with the world for the render to work

	WeaponName = "Unnamed Weapon";

	InitialiseWeaponStats();
	InitialiseStaticMesh();
	InitialiseSounds();

	MuzzleFlashTemplate = nullptr;
	MuzzleFlash = nullptr;
}

void UBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MuzzleFlashTemplate && !MuzzleFlash)
	{
		MuzzleFlash = UGameplayStatics::SpawnEmitterAttached(MuzzleFlashTemplate, this, NAME_None, ProjectileSpawnOffset);
		MuzzleFlash->bSuppressSpawning = true;
	}
}

void UBaseWeapon::Fire()
{
	if (bCanFire && ProjectileType)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			bCanFire = false;

			FRotator FireRotation = GetComponentRotation();
			FVector SpawnLocation = GetComponentLocation() + FireRotation.RotateVector(ProjectileSpawnOffset);

			for(auto i = 0; i < ProjectilesToSpawnOnFire; i++)
			{
				if(ProjectileMaxSpread > FLT_EPSILON)
				{
					const float randomSpread = ((static_cast<float>(ProjectileMaxSpread) / static_cast<float>(ProjectilesToSpawnOnFire)) * i);
					FireRotation.Yaw += randomSpread;
				}
				ABaseProjectile* proj = World->SpawnActor<ABaseProjectile>(ProjectileType, SpawnLocation, FireRotation);
				proj->SetOwningActor(GetOwner());
			}

			UGameplayStatics::PlaySoundAtLocation(this, ShootSound, SpawnLocation, FireRotation);
			MuzzleFlash->bSuppressSpawning = false;

			World->GetTimerManager().SetTimer(TimerHandle_TimeUntilCanFire, this, &UBaseWeapon::ReEnableCanFire, FireInterval);
		}
	}
}

FString UBaseWeapon::GetWeaponName()
{
	return WeaponName;
}

void UBaseWeapon::SetOffset(FVector offset)
{
	AddLocalOffset(WeaponPositionOffset + offset, false);
}

void UBaseWeapon::ReEnableCanFire()
{
	bCanFire = true;
	MuzzleFlash->bSuppressSpawning = true; //TODO: Might have some issues with slower firing weapons turn it off here and not sooner
}

void UBaseWeapon::InitialiseSounds()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> shootCue(TEXT("/Game/Audio/pew_cue"));
	ShootSound = shootCue.Object;
}

void UBaseWeapon::InitialiseStaticMesh()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	SetStaticMesh(WeaponMesh.Object);

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	AddLocalOffset(WeaponPositionOffset, false);
	SetWorldScale3D(FVector(0.75, 0.75, 0.75));
}

void UBaseWeapon::InitialiseWeaponStats()
{
	bCanFire = true;

	FireInterval = 0.2f;
	ProjectileMaxSpread = 0;
	ProjectilesToSpawnOnFire = 1;
	ProjectileSpawnOffset = FVector(0, 0, 0);
	WeaponPositionOffset = FVector(0, 0, 0);
}


void * UBaseWeapon::operator new(std::size_t count)
{
	return nullptr;
}