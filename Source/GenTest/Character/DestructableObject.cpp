#include "DestructableObject.h"

void ADestructableObject::OnDeath()
{
}

ADestructableObject::ADestructableObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADestructableObject::TakeDamage(int value)
{
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

