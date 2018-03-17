#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Health.h"
#include "DestructableObject.generated.h"

UCLASS()
class GENTEST_API ADestructableObject : public AActor, public IHealth
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Health;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "WhenDestroyed"))
	void ReceiveOnDeath();

	void OnDeath() override;
public:	
	ADestructableObject();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int value) override;
	UFUNCTION(BlueprintCallable)
	void Heal(int value) override;

	virtual void Tick(float DeltaTime) override;
	
};
