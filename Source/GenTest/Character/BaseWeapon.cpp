#include "BaseWeapon.h"
#include "BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include <string>
#include "Kismet/GameplayStatics.h"

UBaseWeapon::UBaseWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponentWithWorld(GetWorld());	// If we use the CreateObject method we must register with the world for the render to work

	InitialiseWeaponStats();
	InitialiseStaticMesh();
	InitialiseSounds();
}

void UBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
					const float randomSpread = ((static_cast<float>(FMath::RandRange(0, 100)) / 100.0f) * (ProjectileMaxSpread * 2)) - ProjectileMaxSpread;
					FireRotation.Yaw += randomSpread;
				}
				ABaseProjectile* proj = World->SpawnActor<ABaseProjectile>(ProjectileType, SpawnLocation, FireRotation);
				proj->SetOwningActor(GetOwner());
			}

			UGameplayStatics::PlaySoundAtLocation(this, ShootSound, SpawnLocation, FireRotation);

			World->GetTimerManager().SetTimer(TimerHandle_TimeUntilCanFire, this, &UBaseWeapon::ReEnableCanFire, FireInterval);
		}
	}
}

void UBaseWeapon::SetOffset(FVector offset)
{
	AddLocalOffset(WeaponPositionOffset + offset, false);
}

void UBaseWeapon::ReEnableCanFire()
{
	bCanFire = true;
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
	SetWorldScale3D(FVector(0.75, 0.75, 0.75)); // TODO: Temporary
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