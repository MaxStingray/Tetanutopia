#include "DestructableObject.h"

void ADestructableObject::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTABLE DEAD"));
}

ADestructableObject::ADestructableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Health = 10;
}

void ADestructableObject::TakeDamage(int value)
{
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTABLE HURT"));
	if (value > 0)
	{
		Health -= value;
		if (Health <= 0)
		{
			OnDeath();
		}
	}
}

void ADestructableObject::Heal(int value)
{
	// We wont need this
}

void ADestructableObject::BeginPlay()
{
	Super::BeginPlay();
}

void ADestructableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

