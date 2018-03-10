#include "WeaponPickup.h"
#include "PlayerRobot.h"

void AWeaponPickup::WhileOverlap(AActor * OtherActor)
{
	if (Weapon == nullptr)
	{
		Destroy();
	}

	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	if (player->bPickingPrimaryWeapon)
	{
		TSubclassOf<UBaseWeapon> temp = player->GetPrimaryWeaponType();
		player->EquipWeaponPrimary(WeaponType);
		
		SetWeapon(temp);
	}
	else if (player->bPickingAlternateWeapon)
	{
		TSubclassOf<UBaseWeapon> temp = player->GetAlternateWeaponType();
		player->EquipWeaponAlternate(WeaponType);

		SetWeapon(temp);
	}
}

AWeaponPickup::AWeaponPickup()
{
	Weapon = nullptr;
}

void AWeaponPickup::SetWeapon(TSubclassOf<UBaseWeapon> newWeapon)
{
	if (Weapon) { Weapon->DestroyComponent(); };
	if (newWeapon)
	{
		WeaponType = newWeapon;

		Weapon = NewObject<UBaseWeapon>(this, newWeapon);
		Weapon->AttachTo(RootComponent);
		Weapon->SetupAttachment(RootComponent);
		Weapon->SetCollisionProfileName("NoCollision");
		StartPickupCooldown();
	}

	if (Weapon == nullptr)
	{
		Destroy();
	}
}
