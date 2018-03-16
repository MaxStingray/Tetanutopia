#include "ProjectileDamageComponent.h"
#include "Character/Health.h"

UProjectileDamageComponent::UProjectileDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bPiercesActors = false;
	Damage = 20;

	ActorTypesToIgnore = TArray<TSubclassOf<AActor>>();
}

void UProjectileDamageComponent::CollideWith(AActor* actor)
{
	bool shouldIgnore = false;
	for (TSubclassOf<AActor> type : ActorTypesToIgnore)
	{
		if (actor->StaticClass() == type)
		{
			shouldIgnore = true;
			break;
		}
	}

	if (!shouldIgnore)
	{
		IHealth* healthActor = Cast<IHealth>(actor);
		if (healthActor)
		{
			healthActor->TakeDamage(Damage);
		}

		if (!bPiercesActors)
		{
			GetOwner()->Destroy();
		}
	}
}

void UProjectileDamageComponent::SetDamage(int value)
{
	Damage = value;
}

void UProjectileDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProjectileDamageComponent::SetIfBulletPierces(bool pierces)
{
	bPiercesActors = pierces;
}

void UProjectileDamageComponent::IgnoreTypeOfActor(TSubclassOf<AActor> type)
{
	ActorTypesToIgnore.Add(type);
}

void UProjectileDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

