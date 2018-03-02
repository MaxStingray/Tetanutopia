

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/BaseWeapon.h"
#include "Character/Health.h"
#include "BasicAI.generated.h"

UCLASS()
class GENTEST_API ABasicAI : public ACharacter, public IHealth
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

	int health = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void Attack();

	virtual void OnDeath() override;

	virtual void TakeDamage(int value) override;

	virtual void Heal(int value) override;
};
