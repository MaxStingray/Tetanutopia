#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "ProjectileDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENTEST_API UProjectileDamageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = "_Projectile", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Damage;

	UPROPERTY(Category = "_Projectile", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bPiercesActors;

	TArray<TSubclassOf<AActor>> ActorTypesToIgnore;

protected:
	virtual void BeginPlay() override;

public:	
	UProjectileDamageComponent();

	UFUNCTION(BlueprintCallable)
	void CollideWith(AActor* actor);

	UFUNCTION(BlueprintCallable)
	void SetDamage(int value);

	UFUNCTION(BlueprintCallable)
	void SetIfBulletPierces(bool pierces);

	UFUNCTION(BlueprintCallable)
	void IgnoreTypeOfActor(TSubclassOf<AActor> type);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
