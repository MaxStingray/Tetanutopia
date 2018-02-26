#pragma once

#include "CoreMinimal.h"
#include "CollectableComponent.generated.h"

UENUM(BlueprintType)
enum class ESlotEnum : uint8
{
	SE_Primary 		UMETA(DisplayName = "Primary Weapon"),
	SE_Alternate	UMETA(DisplayName = "Alternate Weapon"),
	SE_Active		UMETA(DisplayName = "Active Item"),
	SE_Passive		UMETA(DisplayName = "Passive Item")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCollectableComponent : public UInterface
{
	GENERATED_BODY()
};

class GENTEST_API ICollectableComponent
{
	GENERATED_BODY()

public:
	virtual ESlotEnum GetEquipmentSlot() = 0;
};
