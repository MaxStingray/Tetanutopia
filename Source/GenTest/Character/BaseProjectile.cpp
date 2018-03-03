#include "BaseProjectile.h"
#include "Health.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"

ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileDamage = 20;
	Owner = nullptr;

	InitialiseStaticMesh();
	InitialiseProjectileMove();
}

void ABaseProjectile::InitialiseStaticMesh()
{
	// Setup the Static Mesh Component
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	// Find the mesh and apply it to the Static Mesh Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	ProjectileMeshComponent->SetStaticMesh(ProjectileMesh.Object);
	ProjectileMeshComponent->SetCollisionProfileName(FName("Projectile"));
	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit); // Callback Event for When Collision Occurs
	ProjectileMeshComponent->SetWorldScale3D(FVector(0.25, 0.25, 0.25)); // TODO: Temporary

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
	ProjectileMovement->ProjectileGravityScale = 0.f; // Disables Gravity

	InitialLifeSpan = 3.0f;	// Lifetime of the projectile
}

void ABaseProjectile::SetOwningActor(AActor* owner)
{
	Owner = owner;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if(OtherActor != Owner)
	{
		IHealth* healthActor = Cast<IHealth>(OtherActor);
		if (healthActor)
		{
			healthActor->TakeDamage(ProjectileDamage);
		}
	}

	// End of the lifetime for this projectile
	Destroy();
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

