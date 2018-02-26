#include "BaseWeaponSpread.h"
//#include "BaseProjectile.h"
//
//UBaseWeaponSpread::UBaseWeaponSpread()
//{
//	ProjectileMaxSpread = 75;
//}
//
//void UBaseWeaponSpread::Fire()
//{
//	if (bCanFire && ProjectileType)
//	{
//		bCanFire = false;
//
//		UWorld* const World = GetWorld();
//
//		const FRotator FireRotation = GetComponentRotation();
//		const FVector SpawnLocation = GetComponentLocation() + FireRotation.RotateVector(ProjectileSpawnOffset) + FVector(FMath::RandRange(-ProjectileMaxSpread, ProjectileMaxSpread), 0, 0);
//
//		if (World != NULL)
//		{
//			// spawn the projectile
//			World->SpawnActor<ABaseProjectile>(ProjectileType, SpawnLocation, FireRotation);
//		}
//
//		World->GetTimerManager().SetTimer(TimerHandle_TimeUntilCanFire, this, &UBaseWeaponSpread::ReEnableCanFire, FireInterval);
//	}
//}