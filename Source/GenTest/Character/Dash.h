#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Components/ActorComponent.h"
#include "PlayerRobot.h"
#include "Dash.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENTEST_API UDash : public UBaseItem
{
	GENERATED_BODY()

private:
	bool bCanDash;
	bool bDashing;
	FTimerHandle TimerHandle_DashCooldown;
	FTimerHandle TimerHandle_Dash;
	APlayerRobot* OwningPlayer;

	UPROPERTY(Category = "Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DashCooldown;

	UPROPERTY(Category = "Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DashSpeedMult;

	UPROPERTY(Category = "Item", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DashRevolutionTime;

	void StopDashing();

public:	
	UDash();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
	UFUNCTION()
	void EnableDash();

	void Use() override;
};
