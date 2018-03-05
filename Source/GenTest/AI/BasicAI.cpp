#include "BasicAI.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABasicAI::ABasicAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the Hurt Sound
	static ConstructorHelpers::FObjectFinder<USoundCue> hurtCue(TEXT("/Game/Audio/Sounds/Hit_Short_Cue"));
	HurtSound = hurtCue.Object;
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
	if (weapon) {
		weapon->Fire();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Weapon"));
	}
}

void ABasicAI::OnDeath()
{
	UE_LOG(LogTemp, Display, TEXT("AI has died"));
	this->Destroy();
}

void ABasicAI::TakeDamage(int value)
{
	UE_LOG(LogTemp, Display, TEXT("AI took damage"));

	UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation(), GetActorRotation());

	health -= value;
	if (health < 0) {
		OnDeath();
	}
}

void ABasicAI::Heal(int value)
{
	health += value;
	if (health > 100) {
		health = 100;
	}
}

void ABasicAI::EquipWeapon(TSubclassOf<UBaseWeapon> weaponType)
{
	if (weapon != nullptr)
	{
		weapon->DestroyComponent();
	}

	weapon = NewObject<UBaseWeapon>(this, weaponType);
	weapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	weapon->SetOffset(FVector(0.0f, 0.0f, 0.0f));
}