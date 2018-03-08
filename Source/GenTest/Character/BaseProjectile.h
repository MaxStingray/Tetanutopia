#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class GENTEST_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

private:
	AActor* ActorWhichFired;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ProjectileDamage;

	UPROPERTY(Category = "Projectile", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bProjectilePiercesActors;

	void InitialiseStaticMesh();
	void InitialiseProjectileMove();

public:
	ABaseProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void SetFiringActor(AActor* shooter);

	virtual void Tick(float DeltaSeconds);
};
