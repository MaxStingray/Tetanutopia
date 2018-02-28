

#include "OutDoorLevelGen.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values
AOutDoorLevelGen::AOutDoorLevelGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOutDoorLevelGen::BeginPlay()
{
	Super::BeginPlay();
	Wall = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Wall->RegisterComponent();
	Wall->SetStaticMesh(rock);
	Wall->SetFlags(RF_Transactional);

	Noise = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Noise->RegisterComponent();
	Noise->SetStaticMesh(spike);
	Noise->SetFlags(RF_Transactional);

	CreateWall();
	AddNoise();
	
}

// Called every frame
void AOutDoorLevelGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//https://www.youtube.com/watch?v=nERpEOABJJo
//usful tutorial for outdoor landscape material

void AOutDoorLevelGen::AddNoise() {
	for (int i = 0; i < 40; i++) {
		int r = FMath::RandRange(0, 100);
		if (r > 0) {
			FVector loc((rand() % 4000 + 1), (rand() % 10000 + 1), 0);
			float r = FMath::FRandRange(0.8f, 2.0f);
			FVector scale(r, r, r);
			FTransform trans(FRotator(0, FMath::RandRange(0, 359), 0), loc, scale);
			SpawnISM(trans, Noise);
		}
		else {
		}
	}
}


void AOutDoorLevelGen::CreateWall() {
	for (int i = 0; i < width; i+=8) {
		float rot = FMath::RandRange(0, 60);
		if (FMath::RandBool()) {
			rot += 180;
		}
		FTransform trans(FRotator(0, rot, 0), FVector(i * 100, 0, FMath::FRandRange(10, 50) * -1), FVector(3,3,3));
		SpawnISM(trans, Wall);
	}

	for (int i = 0; i < width; i+=8) {
		float rot = FMath::RandRange(0, 60);
		if (FMath::RandBool()) {
			rot += 180;
		}
		FTransform trans(FRotator(0, rot, 0), FVector(i * 100, height*100, -100), FVector(3, 3, 3));
		SpawnISM(trans, Wall);
	}

	for (int i = 0; i < height; i+=8) {
		float rot = FMath::RandRange(110, 180);
		if (FMath::RandBool()) {
			rot += 180;
		}
		FTransform trans(FRotator(0, rot, 0), FVector(0, i * 100, FMath::FRandRange(10, 50)), FVector(3, 3, 3));
		SpawnISM(trans, Wall);
	}

	for (int i = 0; i < height; i+=8) {
		float rot = FMath::RandRange(110, 180);
		if (FMath::RandBool()) {
			rot += 180;
		}
		FTransform trans(FRotator(0, rot, 0), FVector(width*100, i * 100, FMath::FRandRange(10,50)), FVector(3, 3, 3));
		SpawnISM(trans, Wall);
	}
}
