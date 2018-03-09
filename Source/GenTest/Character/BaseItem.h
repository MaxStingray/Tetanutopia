#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseItem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENTEST_API UBaseItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Use() { return; };

};
