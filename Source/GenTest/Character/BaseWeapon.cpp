#include "BaseWeapon.h"
#include "BaseProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include <string>

UBaseWeapon::UBaseWeapon()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponentWithWorld(GetWorld());	// If we use the CreateObject method we must register with the world for the render to work

	InitialiseWeaponStats();
	InitialiseStaticMesh();
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
		bCanFire = false;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FRotator FireRotation = GetComponentRotation();
			FVector SpawnLocation = GetComponentLocation() + FireRotation.RotateVector(ProjectileSpawnOffset);

			for(auto i = 0; i < ProjectilesToSpawnOnFire; i++)
			{
				if(ProjectileMaxSpread > FLT_EPSILON)
				{
					const float randomSpread = ((static_cast<float>(FMath::RandRange(0, 100)) / 100.0f) * (ProjectileMaxSpread * 2)) - ProjectileMaxSpread;
					FireRotation.Yaw += randomSpread;
				}
				World->SpawnActor<ABaseProjectile>(ProjectileType, SpawnLocation, FireRotation);
			}

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
