#include "HealthCollectable.h"
#include "Components/SphereComponent.h"
#include "PlayerRobot.h"
#include "Engine/CollisionProfile.h"

AHealthCollectable::AHealthCollectable()
{
	HealthValue = 50;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	CollisionSphere->InitSphereRadius(110.0f);
	CollisionSphere->SetupAttachment(RootComponent);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AHealthCollectable::OnOverlap);
}

void AHealthCollectable::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayerRobot::StaticClass()))
	{
		APlayerRobot* player = Cast<APlayerRobot>(OtherActor);
		if (player)
		{
			player->Heal(HealthValue);
		}
	}
}
