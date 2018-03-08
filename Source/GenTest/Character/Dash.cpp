#include "Dash.h"

UDash::UDash()
{
}

void UDash::BeginPlay()
{
	Super::BeginPlay();	
}

void UDash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

