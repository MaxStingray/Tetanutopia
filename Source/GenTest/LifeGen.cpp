// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeGen.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine.h"


// Sets default values
ALifeGen::ALifeGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALifeGen::CreateMap() {
	const int width = 50;
	const int height = 50;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (rand()%(100+1) < this->livingChance) {
				map[x][y] = true;
			}
		}
	}
	for (int i = 0; i < steps; i++) {
		DoSimulationStep();
	}
}

void ALifeGen::DrawMap()
{
	const int width = 50;
	const int height = 50;
	CreateMap();
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (map[x][y]) {
				SpawnISM(FVector(x * 100, y * 100, 0));
			}
		}
	}
}


int ALifeGen::CountAliveNeighbours(int x, int y) {
	const int width = 50;
	const int height = 50;
	int count = 0;
	for (int i = -1; i<2; i++) {
		for (int j = -1; j<2; j++) {
			int neighbour_x = x + i;
			int neighbour_y = y + j;
			//If we're looking at the middle point
			if (i == 0 && j == 0) {
				//Do nothing, we don't want to add ourselves in!
			}
			//In case the index we're looking at it off the edge of the map
			else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= width || neighbour_y >= height) {
				count = count + 1;
			}
			//Otherwise, a normal check of the neighbour
			else if (map[neighbour_x][neighbour_y]) {
				count = count + 1;
			}
		}
	}
	return count;
}


void ALifeGen::DoSimulationStep() {
	const int width = 50;
	const int height = 50;
	bool tempMap[width][height];
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int nbs = CountAliveNeighbours(x, y);
			//The new value is based on our simulation rules
			//First, if a cell is alive but has too few neighbours, kill it.
			if (map[x][y]) {
				if (nbs < deathLimit) {
					tempMap[x][y] = false;
				}
				else {
					tempMap[x][y] = true;
				}
			} //Otherwise, if the cell is dead now, check if it has the right number of neighbours to be 'born'
			else {
				if (nbs > birthLimit) {
					tempMap[x][y] = true;
				}
				else {
					tempMap[x][y] = false;
				}
			}
		}
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			map[i][j] = tempMap[i][j];
		}
	}
}

// Called when the game starts or when spawned
void ALifeGen::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALifeGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALifeGen::DeleteRandomBlock() {TArray<UInstancedStaticMeshComponent*> Comps; //this gets an array of the instance static mesh components. 
	/*
	GetComponents(Comps); // populates comps variable
	UInstancedStaticMeshComponent* FoundComp;
	if (Comps.Num() > 0) //loops through comps if we have more than one
	{
		FoundComp = Comps[0]; // get the indevidual instance we want. Not sure how to tell them appart yet
	}
	FTransform t2[1000]; // create empty transform to store data
	for (int i = 0; i < FoundComp->GetInstanceCount(); i++) {
		FoundComp->GetInstanceTransform(i, t2[i], true);
	}
	*/
}

