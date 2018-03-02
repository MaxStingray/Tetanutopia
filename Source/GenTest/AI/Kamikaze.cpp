

#include "Kamikaze.h"


// Sets default values
AKamikaze::AKamikaze()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKamikaze::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKamikaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKamikaze::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKamikaze::Attack() {
	this->weapon->Fire();
	OnDeath();
	UE_LOG(LogTemp, Display, TEXT("Kamikaze exploded"));
}

