

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/BaseWeapon.h"
#include "Character/Health.h"
#include "MasterAI.generated.h"

UCLASS()
class GENTEST_API AMasterAI : public ACharacter, public IHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMasterAI();

	// I dont expose this because it only works if you set it through the function
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBaseWeapon* weapon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The sound that plays when a the AI take damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundCue* HurtSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxHealth = 100;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual	void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void OnDeath() override;
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(int value) override;
	UFUNCTION(BlueprintCallable)
	virtual void Heal(int value) override;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<UBaseWeapon> weaponType);
};
