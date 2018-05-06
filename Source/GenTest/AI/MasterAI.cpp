#include "MasterAI.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMasterAI::AMasterAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the Hurt Sound
	static ConstructorHelpers::FObjectFinder<USoundCue> hurtCue(TEXT("/Game/Audio/Sounds/Hit_Short_Cue"));
	HurtSound = hurtCue.Object;
}



// Called when the game starts or when spawned
void AMasterAI::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMasterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMasterAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMasterAI::Attack() {
	if (weapon) {
		weapon->Fire();
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("No Weapon"));
	}
}

void AMasterAI::OnDeath()
{
	//UE_LOG(LogTemp, Display, TEXT("AI has died"));
	this->ReceiveOnDeath();
}

void AMasterAI::TakeDamage(int value)
{
	//UE_LOG(LogTemp, Display, TEXT("AI took damage"));

	UGameplayStatics::PlaySoundAtLocation(this, HurtSound, GetActorLocation(), GetActorRotation());

	if (!Invulerable) {
		health -= value;
	}
	if (health < 0) {
		OnDeath();
	}
}

void AMasterAI::Heal(int32 value)
{
	health += value;
	if (health > maxHealth) {
		health = maxHealth;
	}
	//UE_LOG(LogTemp, Display, TEXT("AI was healed"));
	ReceiveOnHeal();
}

void AMasterAI::EquipWeapon(TSubclassOf<UBaseWeapon> weaponType)
{
	if (weapon != nullptr)
	{
		weapon->DestroyComponent();
	}

	weapon = NewObject<UBaseWeapon>(this, weaponType);
	weapon->SetOffset(weaponOffset);
	weapon->AttachTo(RootComponent);
	weapon->SetupAttachment(RootComponent);
}