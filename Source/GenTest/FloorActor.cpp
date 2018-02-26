// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorActor.h"


// Sets default values
AFloorActor::AFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay()
{
	Super::BeginPlay();
	Create();
}

// Called every frame
void AFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorActor::Create() {
	SetActorLocation(FVector(10, 10, 0));
}

