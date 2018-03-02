

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicAI.h"
#include "Kamikaze.generated.h"

UCLASS()
class GENTEST_API AKamikaze : public ABasicAI
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKamikaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;
	
};
