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
		//FRotator rot = GetOwner()->GetActorRotation();
		//rot.Roll += (360.0f / DashRevolutionTime * DeltaTime);
		//GetOwner()->SetActorRotation(rot);
		
		FVector oldPos = GetOwner()->GetActorLocation();
		FQuat rot = FQuat(FVector(1,0,0), FMath::DegreesToRadians(360.0f) / DashRevolutionTime * DeltaTime);
		FHitResult Hit(1.f);
		
		GetOwner()->AddActorLocalRotation(rot, true, &Hit);
		GetOwner()->SetActorLocation(oldPos);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(GetOwner()->GetActorLocation() - oldPos, Normal2D) * (1.f - Hit.Time);
			GetOwner()->AddActorWorldOffset(Deflection, true);
		}
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
