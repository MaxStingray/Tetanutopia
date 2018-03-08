#include "BaseProjectile.h"
#include "Health.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/Particles/ParticleSystemComponent.h"
#include "Classes/Particles/ParticleSystem.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bGenerateOverlapEventsDuringLevelStreaming = false;

	bProjectilePiercesActors = false;
	ProjectileDamage = 20;

	ActorWhichFired = nullptr;

	InitialiseStaticMesh();
	InitialiseProjectileMove();
}

void ABaseProjectile::InitialiseStaticMesh()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));

	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMeshComponent->SetStaticMesh(ProjectileMesh.Object);
	ProjectileMeshComponent->SetWorldScale3D(FVector(0.25, 0.25, 0.25));

	ProjectileMeshComponent->SetCollisionProfileName(FName("Projectile"));
	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	ProjectileMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);

	ProjectileMeshComponent->bGenerateOverlapEvents = true;
	ProjectileMeshComponent->SetNotifyRigidBodyCollision(true);

	RootComponent = ProjectileMeshComponent;
}

void ABaseProjectile::InitialiseProjectileMove()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ProjectileMeshComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	InitialLifeSpan = 2.0f;
}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != ActorWhichFired)
	{
		IHealth* healthActor = Cast<IHealth>(OtherActor);
		if (healthActor)
		{
			healthActor->TakeDamage(ProjectileDamage);
		}

		if (!bProjectilePiercesActors)
		{
			Destroy();
		}
	}
}

void ABaseProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	Destroy();
}

void ABaseProjectile::SetFiringActor(AActor* owner)
{
	if (owner != nullptr)
	{
		ActorWhichFired = owner;
	}
}

void ABaseProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

