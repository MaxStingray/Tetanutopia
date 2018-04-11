#include "HealthPickup.h"
#include "PlayerRobot.h"

AHealthPickup::AHealthPickup()
{
	HealthToGrant = 50;
}

void AHealthPickup::WhileOverlap(AActor* OtherActor)
{
	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	
	if (player->GetCurrentHealth() < player->GetMaxHealth())
	{
		player->Heal(HealthToGrant);
		Destroy();
	}
}