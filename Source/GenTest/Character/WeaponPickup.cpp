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
		UBaseWeapon* temp = player->GetPrimaryWeapon();
		player->EquipWeaponPrimary(Weapon->StaticClass());
		
		SetWeapon(temp->StaticClass());
	}
	else if (player->bPickingAlternateWeapon)
	{
		UBaseWeapon* temp = player->GetAlternateWeapon();
		player->EquipWeaponAlternate(Weapon->StaticClass());

		SetWeapon(temp->StaticClass());
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
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Cast<UBaseWeapon>(newWeapon)->GetWeaponName());
		Weapon = NewObject<UBaseWeapon>(this, newWeapon);
		Weapon->AttachTo(RootComponent);
		Weapon->SetupAttachment(RootComponent);
		Weapon->SetCollisionProfileName("NoCollision");
	}
}
