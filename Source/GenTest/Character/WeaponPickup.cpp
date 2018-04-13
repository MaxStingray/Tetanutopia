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

		player->bDisplayWeaponPickup = false;
		
		SetWeapon(temp);
	}
	else if (player->bPickingAlternateWeapon)
	{
		TSubclassOf<UBaseWeapon> temp = player->GetAlternateWeaponType();
		player->EquipWeaponAlternate(WeaponType);

		player->bDisplayWeaponPickup = false;

		SetWeapon(temp);
	}
	else
	{
		player->bDisplayWeaponPickup = true;
		player->PickupContext = Weapon->GetWeaponName();
	}
}

void AWeaponPickup::OverlapEnds(AActor* OtherActor)
{
	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	player->bDisplayWeaponPickup = false;
	player->PickupContext = "";
}

AWeaponPickup::AWeaponPickup()
{
	Weapon = nullptr;
}

void AWeaponPickup::SetWeapon(TSubclassOf<UBaseWeapon> newWeapon)
{
	if (newWeapon != WeaponType)
	{
		if (newWeapon.Get() == nullptr)
		{
			Destroy();
			return;
		}

		if (Weapon) { Weapon->DestroyComponent(); };
		if (newWeapon)
		{
			WeaponType = newWeapon;

			Weapon = NewObject<UBaseWeapon>(this, newWeapon);
			Weapon->AttachTo(RootComponent);
			Weapon->SetupAttachment(RootComponent);
			Weapon->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
			Weapon->SetCollisionProfileName("NoCollision");
			StartPickupCooldown();
		}
	}
}
