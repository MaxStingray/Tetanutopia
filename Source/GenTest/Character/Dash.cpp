#include "Dash.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

void UDash::StopDashing()
{
	bDashing = false;

	FRotator rot = GetOwner()->GetActorRotation();
	rot.Roll = 0;
	GetOwner()->SetActorRotation(rot);

	OwningPlayer->MoveSpeedMult -= DashSpeedMult;
	//OwningPlayer->bIsVulnerable = true;
	OwningPlayer->SetInputActive(true);
}

UDash::UDash()
{
	PrimaryComponentTick.bCanEverTick = true;

	bCanDash = true;
	bDashing = false;
	DashCooldown = 1.5f;
	DashSpeedMult = 0.65f;
	DashRevolutionTime = 0.5f;

	ItemName = "Dash";
}

void UDash::BeginPlay()
{
	Super::BeginPlay();	

	OwningPlayer = Cast<APlayerRobot>(GetOwner());
	if (!OwningPlayer)
	{
		DestroyComponent();
	}
}

void UDash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If we can't dash, that means we are dashing
	if (bDashing)
	{
		FRotator rot = GetOwner()->GetActorRotation();
		rot.Roll += (360.0f / DashRevolutionTime * DeltaTime);
		GetOwner()->SetActorRotation(rot);
	}
}

void UDash::EnableDash()
{
	bCanDash = true;
}

void UDash::Use()
{
	if (bCanDash && !bDashing && OwningPlayer->bIsMoving)
	{
		bCanDash = false;
		bDashing = true;

		OwningPlayer->SetInputActive(false);
		OwningPlayer->MoveSpeedMult += DashSpeedMult;
		//OwningPlayer->bIsVulnerable = false;
		OwningPlayer->MakeInvulnerable(DashRevolutionTime);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashCooldown, this, &UDash::EnableDash, DashCooldown);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Dash, this, &UDash::StopDashing, DashRevolutionTime);

		ReceiveDashStart();
	}
}

float UDash::GetMaxCooldown()
{
	return DashCooldown;
}

float UDash::GetRemainingCooldown()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_DashCooldown);
}
