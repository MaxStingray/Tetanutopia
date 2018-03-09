#include "BaseItem.h"

UBaseItem::UBaseItem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseItem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

