#include "HealthPickup.h"
#include "PlayerRobot.h"

AHealthPickup::AHealthPickup()
{
	HealthToGrant = 50;
}

void AHealthPickup::WhileOverlap(AActor* OtherActor)
{
	APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
	player->Heal(HealthToGrant);

	Destroy();
}