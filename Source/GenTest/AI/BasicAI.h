

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Character/BaseWeapon.h"
#include "BasicAI.generated.h"

UCLASS()
class GENTEST_API ABasicAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicAI();

	UPROPERTY(Category = "Weapons", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBaseWeapon* weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Attack();
	
};
