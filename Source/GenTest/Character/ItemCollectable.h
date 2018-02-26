#pragma once

#include "CoreMinimal.h"
#include "Character/Collectable.h"
#include "ItemCollectable.generated.h"

UCLASS()
class GENTEST_API AItemCollectable : public ACollectable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnOverlap() override;
};
