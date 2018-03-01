#include "Health.h"

void IHealth::OnDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Actor implementing IHealth died! This is the interface method however!"));
}