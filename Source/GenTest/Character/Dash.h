#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dash.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENTEST_API UDash : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDash();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
};
