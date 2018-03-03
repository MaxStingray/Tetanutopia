// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "MyActor.h"//include so it knows what this is
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "BSP.h"


// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void * ALevelManager::operator new(size_t size)
{
	return nullptr;
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateLevel();
	CreatePath();
	/*
	TArray<ABSP> rooms; // flat rectangle store to help pick a random one
	ABSP root(0, 0, 60, 120); 
	rooms.Add(root); //populate rectangle store with root area

	while (rooms.Num() < 19) { // this will give us 10 leaf areas
		int splitIdx = rand() % (rooms.Num() + 1); // choose a random element
		ABSP* toSplit = &rooms[splitIdx];
		if (toSplit->split()) { //attempt to split
			rooms.Add(*toSplit->leftChild);
			rooms.Add(*toSplit->rightChild);
		}

	}
	root.generateDungeon(); //generate dungeons*/

}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelManager::GenerateLevel() {
	int misscount = 2;
	int percent = 60;
	for (int x = 0; x < roomsX; x++) {
		for (int y = 0; y < roomsY; y++) {
			int r = rand() % 100+1;
			if (r > percent && misscount != 0 && (x != 0 && y != 0)) {
				Rooms[x][y] = nullptr;
				misscount--;
			}
			else {
				Rooms[x][y] = Spawn(FVector(x*roomXSize*unitSize, y*roomYSize*unitSize, 0));
				Rooms[x][y]->x = x;
				Rooms[x][y]->y = y;
			}
		}
	}
}

ARoomManager* ALevelManager::Spawn(FVector location)
{
	if (rm)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			return world->SpawnActor<ARoomManager>(rm, location, FRotator::ZeroRotator, spawnParams);
		}
	}
	return nullptr;
}

void ALevelManager::CreatePath() {
	ARoomManager* current = Rooms[0][0];
	bool unvisited = true;
	int stackCounter = 0;
	ARoomManager* stack[roomsX*roomsY];

	while (unvisited) {
		unvisited = false;
		current->visited = true;
		ARoomManager* next = CheckNeighbours(current->x, current->y);

		if (next != nullptr) {
			stack[stackCounter] = current;
			AddDoors(current->x, current->y, next->x, next->y);
			next->visited = true;
			current = next;
			stackCounter++;
		}
		else if (stackCounter > 0) {
			stack[stackCounter] = nullptr;
			stackCounter--;
			current = stack[stackCounter];
		}

		//check to see if there are any rooms left unvisited
		for (int i = 0; i < roomsX; i++) {
			for (int j = 0; j < roomsY; j++) {
				if (Rooms[i][j] != nullptr) {
					if (!Rooms[i][j]->visited) {
						unvisited = true;
					}
				}
			}
		}
	}
}

ARoomManager* ALevelManager::CheckNeighbours(int x, int y) {
	ARoomManager* neighbours[4];

	if (y < roomsY-1 && Rooms[x][y + 1] != nullptr) {
		if (!Rooms[x][y + 1]->visited) {
			neighbours[0] = Rooms[x][y + 1];
		}
		else {
			neighbours[0] = nullptr;
		}
	}
	else {
		neighbours[0] = nullptr;
	}

	if (x < roomsX - 1 && Rooms[x+1][y] != nullptr) {
		if (!Rooms[x + 1][y]->visited) {
			neighbours[1] = Rooms[x + 1][y];
		}
		else {
			neighbours[1] = nullptr;
		}
	}
	else {
		neighbours[1] = nullptr;
	}

	if (y >= 1 && Rooms[x][y - 1] != nullptr) {
		if (!Rooms[x][y - 1]->visited) {
			neighbours[2] = Rooms[x][y - 1];
		}
		else {
			neighbours[2] = nullptr;
		}
	}
	else {
		neighbours[2] = nullptr;
	}

	if (x >= 1 && Rooms[x - 1][y] != nullptr) {
		if (!Rooms[x - 1][y]->visited) {
			neighbours[3] = Rooms[x - 1][y];
		}
		else {
			neighbours[3] = nullptr;
		}
	}
	else {
		neighbours[3] = nullptr;
	}

	if (neighbours[0] == nullptr && neighbours[1] == nullptr && neighbours[2] == nullptr && neighbours[3] == nullptr) {
		return nullptr;
	}

	int r;
	ARoomManager* passBack = nullptr;
	while (passBack == nullptr) {
		r = rand() % (3 + 1);
		passBack = neighbours[r];
	}
	
	return passBack;

}

void ALevelManager::AddDoors(int x1, int y1, int x2, int y2) {
	int x = x1 - x2;
	if (x == 1) {
		int r = rand() % (roomYSize - 3);
		if (r <= 2) {
			r += 2;
		}
		FTransform newDoor;
		Rooms[x1][y1]->Walls->GetInstanceTransform(roomXSize + roomYSize + roomXSize + r, newDoor);
		Rooms[x1][y1]->Spawn(FVector(newDoor.GetLocation().X - (unitSize / 2), newDoor.GetLocation().Y - (unitSize / 2), newDoor.GetLocation().Z), Door);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + roomYSize + roomXSize + r);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + roomYSize + roomXSize + r - 1);

		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + r);
		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + r - 1);

	}
	else if (x == -1) {
		int r = rand() % (roomYSize - 3);
		if (r <= 2) {
			r += 2;
		}
		FTransform newDoor;
		Rooms[x1][y1]->Walls->GetInstanceTransform(roomXSize + r, newDoor);
		Rooms[x1][y1]->Spawn(FVector(newDoor.GetLocation().X + (unitSize / 2), newDoor.GetLocation().Y - (unitSize / 2), newDoor.GetLocation().Z), Door);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + r);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + r - 1);

		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + roomYSize + roomXSize + r);
		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + roomYSize + roomXSize + r - 1);

	}

	int y = y1 - y2;
	if (y == 1) {
		int r = rand() % (roomXSize - 3);
		if (r <= 2) {
			r += 2;
		}
		FTransform newDoor;
		Rooms[x1][y1]->Walls->GetInstanceTransform(r, newDoor);
		FRotator rot(0,90,0);
		FTransform newPosition = FTransform(rot, FVector(newDoor.GetLocation().X - (unitSize / 2), newDoor.GetLocation().Y - (unitSize / 2), newDoor.GetLocation().Z), FVector(1, 1, 1));
		Rooms[x1][y1]->SpawnTransform(newPosition, Door);
		Rooms[x1][y1]->Walls->RemoveInstance(r);
		Rooms[x1][y1]->Walls->RemoveInstance(r - 1);

		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + roomYSize + r);
		Rooms[x2][y2]->Walls->RemoveInstance(roomXSize + roomYSize + r - 1);

	}
	else if (y == -1) {
		int r = rand() % (roomXSize - 3);
		if (r <= 2) {
			r += 2;
		}
		FTransform newDoor;
		Rooms[x1][y1]->Walls->GetInstanceTransform(roomXSize + roomYSize + r, newDoor);
		FRotator rot(0, 90, 0);
		FTransform newPosition = FTransform(rot, FVector(newDoor.GetLocation().X - (unitSize / 2), newDoor.GetLocation().Y + (unitSize / 2), newDoor.GetLocation().Z), FVector(1, 1, 1));
		Rooms[x1][y1]->SpawnTransform(newPosition, Door);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + roomYSize + r);
		Rooms[x1][y1]->Walls->RemoveInstance(roomXSize + roomYSize + r - 1);

		Rooms[x2][y2]->Walls->RemoveInstance(r);
		Rooms[x2][y2]->Walls->RemoveInstance(r - 1);

	}
}
