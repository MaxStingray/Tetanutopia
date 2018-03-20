#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseItem.generated.h"

UCLASS(Blueprintable)
class GENTEST_API UBaseItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Category = "_Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ItemName;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual void Use() { return; };

	UFUNCTION(BlueprintCallable)
	FString GetWeaponName();

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxCooldown() { return -1.0f; };

	UFUNCTION(BlueprintCallable)
	virtual float GetRemainingCooldown() { return -1.0f; };
};
