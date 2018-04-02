#include "ItemPickup.h"
#include "PlayerRobot.h"

void AItemPickup::WhileOverlap(AActor * OtherActor)
{
	if (Item == nullptr)
	{
		Destroy();
	}

	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	if (player->bPickingUpItem)
	{
		TSubclassOf<UBaseItem> temp = player->GetItemType();
		player->EquipItem(Item);
		SetItem(temp);

		player->bDisplayItemPickup = false;
	}
	else
	{
		player->bDisplayItemPickup = true;

		UBaseItem* temp = NewObject<UBaseItem>(this, Item);
		player->PickupContext = temp->GetWeaponName();
		temp->DestroyComponent();
	}
}

void AItemPickup::OverlapEnds(AActor* OtherActor)
{
	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	player->bDisplayItemPickup = false;
	player->PickupContext = "";
}

AItemPickup::AItemPickup()
{
}

void AItemPickup::SetItem(TSubclassOf<UBaseItem> newItem)
{
	Item = newItem;
	StartPickupCooldown();

	if (Item == nullptr || Item == NULL || Item.Get() == nullptr || Item.Get() == NULL)
	{
		Destroy();
	}
}