#pragma once
#include "CoreMinimal.h"
#include "Health.generated.h"

UINTERFACE(MinimalAPI)
class UHealth : public UInterface
{
	GENERATED_BODY()
};

class GENTEST_API IHealth
{
	GENERATED_BODY()
private:
	//UFUNCTION(BlueprintCallable)
	virtual void OnDeath();
public:
	//UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(int value) = 0;
	//UFUNCTION(BlueprintCallable)
	virtual void Heal(int value) = 0;
};
