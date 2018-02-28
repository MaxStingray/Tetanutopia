

#include "BasicAI.h"


// Sets default values
ABasicAI::ABasicAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicAI::Attack() {
	weapon->Fire();
}

