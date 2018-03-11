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
	}
}

AItemPickup::AItemPickup()
{
}

void AItemPickup::SetItem(TSubclassOf<UBaseItem> newItem)
{
	Item = newItem;
	StartPickupCooldown();

	if (Item == nullptr)
	{
		Destroy();
	}
}