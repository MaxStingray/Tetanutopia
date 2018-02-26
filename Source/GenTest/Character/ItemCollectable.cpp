#include "ItemCollectable.h"
#include "Engine/Engine.h"
#include "PlayerRobot.h"

void AItemCollectable::OnOverlap()
{
	if(OverlappingActor)
	{
		APlayerRobot* player = Cast<APlayerRobot>(OverlappingActor);
		if (player->bPickingUpItem)
		{
			TSubclassOf<UStaticMeshComponent> temp = player->ItemType;
			player->EquipItemActive(ItemType);

			ItemType = temp;
			if(ItemType == nullptr)
			{
				// The player hasnt swapped item, merely picked it up
				Destroy();
			}

			SetItem();
			bPickupAble = false;

			UWorld* const World = GetWorld();
			World->GetTimerManager().SetTimer(TimerHandle_CollectCooldown, this, &AItemCollectable::EnablePickup, PickupCooldown);
		}
		else
		{
			const FString message = FString("Press X to Pickup Item");
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, message);
		}
	}
}


