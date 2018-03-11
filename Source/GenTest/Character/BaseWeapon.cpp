#include "BaseWeapon.h"
#include "BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Engine/World.h"

UBaseWeapon::UBaseWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponentWithWorld(GetWorld());	// If we use the CreateObject method we must register with the world for the render to work

	InitialiseWeaponStats();
	InitialiseStaticMesh();
	InitialiseSounds();

	ProjectileType = ABaseProjectile::StaticClass();

	MuzzleFlashTemplate = nullptr;
	MuzzleFlash = nullptr;
}

void UBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	initialProjectileOffset = ProjectileSpawnOffset;
}

void UBaseWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// This doesnt work in beginplay for some reason
	if (MuzzleFlashTemplate && !MuzzleFlash)
	{
		MuzzleFlash = UGameplayStatics::SpawnEmitterAttached(MuzzleFlashTemplate, this, NAME_None, ProjectileSpawnOffset);
		MuzzleFlash->bSuppressSpawning = true;
	}
}

void UBaseWeapon::Fire()
{
	if (randomiseOffset)
	{
		//randomise the offset
		RandomiseOffset(ProjectileSpawnOffset);
	}
	if (bCanFire && bCanBurstFire && ProjectileType != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			bCanFire = false;
			FRotator FireRotation = GetComponentRotation();
			FVector SpawnLocation = GetComponentLocation() + FireRotation.RotateVector(ProjectileSpawnOffset);

			ProjectileMaxSpread = FGenericPlatformMath::Abs(ProjectileMaxSpread);
			const float ProjectileSpreadSeperation = ProjectileMaxSpread / ProjectilesToSpawnOnFire;
			const float ProjectileSpreadStartOffset = (-1.0f * (ProjectileMaxSpread / 2)) - (ProjectileSpreadSeperation / 2);
			FireRotation.Yaw += ProjectileSpreadStartOffset;

			for(auto i = 0; i < ProjectilesToSpawnOnFire; i++)
			{
				if(ProjectileMaxSpread > FLT_EPSILON)
				{
					FireRotation.Yaw += ProjectileSpreadSeperation;

					if (bSpreadImpactsOffset)
					{
						SpawnLocation = GetComponentLocation() + FireRotation.RotateVector(ProjectileSpawnOffset);
					}
				}

				ABaseProjectile* proj = World->SpawnActorDeferred<ABaseProjectile>(ProjectileType, FTransform(FireRotation, SpawnLocation));				
				if (proj)
				{
					proj->SetFiringActor(GetOwner());

					if (ShootSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, ShootSound, SpawnLocation, FireRotation);
					}
					if (MuzzleFlash != nullptr)
					{
						MuzzleFlash->bSuppressSpawning = false;
					}
					proj->FinishSpawning(FTransform(FireRotation, SpawnLocation));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("There was an issue registering the owner of the projectile"));
				}
			}
			shotCount++;
			World->GetTimerManager().SetTimer(TimerHandle_TimeUntilCanFire, this, &UBaseWeapon::ReEnableCanFire, FireInterval);
			
		}
		if (shotCount >= burstSize)
		{
			bCanBurstFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_TimeUntilBurstFire, this, &UBaseWeapon::ReEnableBurstFire, burstFireInterval);
		}

	}

}

FString UBaseWeapon::GetWeaponName()
{
	if (WeaponName.Len() > 0)
	{
		return WeaponName;
	}
	else
	{
		return "";
	}
	
}

void UBaseWeapon::SetOffset(FVector offset)
{
	AddLocalOffset(WeaponPositionOffset + offset, false);
}

void UBaseWeapon::ReEnableCanFire()
{
	bCanFire = true;
	
	if(MuzzleFlash)
	{
		MuzzleFlash->bSuppressSpawning = true; //TODO: Might have some issues with slower firing weapons turn it off here and not sooner
	}
}

void UBaseWeapon::ReEnableBurstFire()
{
	shotCount = 0;
	bCanBurstFire = true;
}

void UBaseWeapon::InitialiseSounds()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> shootCue(TEXT("/Game/Audio/Sounds/Hit_Shorter1_Cue"));
	ShootSound = shootCue.Object;
}

void UBaseWeapon::InitialiseStaticMesh()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(TEXT("/Game/Geometry/Meshes/Gun"));
	SetStaticMesh(WeaponMesh.Object);

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	AddLocalOffset(WeaponPositionOffset, false);
	SetWorldScale3D(FVector(0.75, 0.75, 0.75));
}

void UBaseWeapon::InitialiseWeaponStats()
{
	bCanFire = true;
	bCanBurstFire = true;
	WeaponName = "Gun";
	bSpreadImpactsOffset = false;

	FireInterval = 1.0f;
	ProjectileMaxSpread = 0;
	ProjectilesToSpawnOnFire = 1;
	ProjectileSpawnOffset = FVector(10, 0, 0);
	WeaponPositionOffset = FVector(0, 0, 0);
}

void UBaseWeapon::RandomiseOffset(FVector offset)
{
	//reset the projectile offset after each fire event
	ProjectileSpawnOffset = initialProjectileOffset;

	float minXOffset = ProjectileSpawnOffset.Y;
	float maxXOffset = ProjectileSpawnOffset.Y * 50;

	float randOffset = FMath::RandRange(minXOffset, maxXOffset);

	ProjectileSpawnOffset = FVector(ProjectileSpawnOffset.X, randOffset, ProjectileSpawnOffset.Z);
}
