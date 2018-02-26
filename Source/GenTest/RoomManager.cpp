// Fill out your copyright notice in the Description page of Project Settings.
//https://docs.unrealengine.com/latest/INT/Programming/Tutorials/Components/1/
//https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Components/UInstancedStaticMeshComponent/

#include "RoomManager.h"
#include "LevelManager.h"
#include <exception>
#include "Runtime/Engine/Classes/Engine/World.h"
#include "EngineUtils.h"
#include "MyActor.h"//include so it knows what this is
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

// Sets default values
ARoomManager::ARoomManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");
	PrimaryActorTick.bCanEverTick = true;

	//https://answers.unrealengine.com/questions/579025/how-to-create-a-cube-manually-from-c-code.html
	//ISMC = NewObject<UInstancedStaticMeshComponent>(this);
	//ISMC->RegisterComponent();
	//UStaticMesh* cube = CreateDefaultSubobject<UStaticMesh>(TEXT("Cube Mesh"));
	//ISMC->SetStaticMesh(cube);
	//ISMC->SetFlags(RF_Transactional);
	//this->AddInstanceComponent(ISMC);
}

void ARoomManager::Spawn(FVector location, TSubclassOf<AActor> actor)
{
	
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			world->SpawnActor<AActor>(actor, location, FRotator::ZeroRotator, spawnParams);
		}
	
}

void ARoomManager::DrawWall(int startX, int startY, int endX, int endY)
{
	//is the line horizontal or vertical?
	bool isHorizontal = false;
	bool isVertical = false;
	bool invalid = false;
	if (startY == endY)
	{
		isHorizontal = true;
		isVertical = false;
	}
	else if (startX == endX)
	{
		isHorizontal = false;
		isVertical = true;
	}
	else
	{
		invalid = true;
	}
	//if the line is valid (horizontal or veritical)
	if (!invalid)
	{
		if (isHorizontal)
		{
			for (int x = startX; x < (startX + endX); x++)
			{
				FString nextSpawnX = FString::FromInt(x) + "," + FString::FromInt(startY);
				if (floorMap.Contains(nextSpawnX))
				{
					FTransform t(FRotator(0, 90, 0), floorMap.FindChecked(nextSpawnX), FVector(1, 1, 1));
					SpawnISM(t, Walls);
				}
			}
		}
		else if (isVertical)
		{
			for (int y = startY; y < (startY + endY); y++)
			{
				FString nextSpawnY = FString::FromInt(startX) + "," + FString::FromInt(y);
				if (floorMap.Contains(nextSpawnY))
				{
					FTransform t(FRotator(0, 180, 0), floorMap.FindChecked(nextSpawnY), FVector(1, 1, 1));
					SpawnISM(t, Walls);
				}
			}
		}
	}
}

void ARoomManager::GenerateRoom(int sizeX, int sizeY)
{
	ALevelManager* lm = new ALevelManager();
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}

	sizeX *= lm->unitSize;
	sizeY *= lm->unitSize;
	FVector spawnLocation = this->GetActorLocation();
	FVector blCorner((spawnLocation.X - (sizeX / 2)), (spawnLocation.Y - (sizeY / 2)), 0);
	//floor loop
	//additional _x and _y counters for local coordinates
	for (int y = blCorner.Y, _y = 0; y < blCorner.Y + sizeY; y += lm->unitSize, _y++)
	{
		for (int x = blCorner.X, _x = 0; x < blCorner.X + sizeX; x += lm->unitSize, _x++)
		{
			//add world coordinates to list 
			floorCoords.Add(FVector(x, y, 0));
			//get local coordinates from _x and _y and make a string
			//string is the key value to access world coordinates from the TMap
			floorMap.Add((FString::FromInt(_x) + "," + FString::FromInt(_y)), FVector(x, y, 0));
			//spawn mesh
			FQuat rot(0, 0, 0, 0);
			rot.Normalize();
			FTransform t(rot, FVector(x, y, 0), FVector(1, 1, 1));
			SpawnISM(t, Floors);
			
		}
	}

	for (int x = blCorner.X; x < blCorner.X + sizeX; x += lm->unitSize)
	{
		wallCoords.Add(FVector(x, blCorner.Y, 0));
		FTransform t(FRotator(0,90,0), FVector(x, blCorner.Y, 0), FVector(1, 1, 1));
		SpawnISM(t, Walls);
	}

	for (int y = blCorner.Y; y < blCorner.Y + sizeY; y += lm->unitSize)
	{
		wallCoords.Add(FVector(blCorner.X + sizeX - lm->unitSize, y, 0));

		FTransform t(FRotator(0, 180, 0), FVector(blCorner.X + sizeX - lm->unitSize, y, 0), FVector(1, 1, 1));

		SpawnISM(t, Walls);
	}

	for (int x = blCorner.X; x < blCorner.X + sizeX; x += lm->unitSize)
	{
		wallCoords.Add(FVector(x, blCorner.Y + sizeY - lm->unitSize, 0));

		FTransform t(FRotator(0, 270, 0), FVector(x, blCorner.Y + sizeY - lm->unitSize, 0), FVector(1, 1, 1));

		SpawnISM(t, Walls);
	}

	for (int y = blCorner.Y; y < blCorner.Y + sizeY - lm->unitSize; y += lm->unitSize)
	{
		wallCoords.Add(FVector(blCorner.X, y, 0));

		FTransform t(FRotator(0, 0, 0), FVector(blCorner.X, y, 0), FVector(1, 1, 1));

		SpawnISM(t, Walls);
	}

}
//creates a rectangular room of a specified height and width
void ARoomManager::DrawInteriorRoom(int startX, int startY, int width, int height)
{
	int endX = startX - width;//might need to be +
	int endY = startY + height;//also might be wrong
	assignedRoomType = RoomType::RT_WEAPON;
	//draw lines between each point
	for (int x = startX; x > (endX)-1; x--)//might need to flip loop!
	{
		//top line
		if (x != endX)
		{
			FString nextSpawnX = FString::FromInt(x) + "," + FString::FromInt(startY);
			if (floorMap.Contains(nextSpawnX))
			{
				FTransform t(FRotator(0, 90, 0), floorMap.FindChecked(nextSpawnX), FVector(1, 1, 1));
				SpawnISM(t, Walls);
			}
		}
		else
		{
			//right line
			for (int y = (endY - height); y < endY + 1; y++)
			{
				FString nextSpawnY = FString::FromInt(endX) + "," + FString::FromInt(y);
				if(floorMap.Contains(nextSpawnY))
				{
					FTransform t(FRotator(0, 180, 0), floorMap.FindChecked(nextSpawnY), FVector(1, 1, 1));
					SpawnISM(t, Walls);
				}
			}
		}

		//bottom line in the other direction
		for (int x = endX; x < startX + 1; x++)
		{
			if (x != startX)
			{
				FString nextSpawnX = FString::FromInt(x) + "," + FString::FromInt(endY);
				if (floorMap.Contains(nextSpawnX))
				{
					FTransform t(FRotator(0, 270, 0), floorMap.FindChecked(nextSpawnX), FVector(1, 1, 1));
					SpawnISM(t, Walls);
				}
			}
			else
			{
				//left line
				for (int y = endY; y > startY - 1; y--)
				{
					FString nextSpawnY = FString::FromInt(startX) + "," + FString::FromInt(y);
					if (floorMap.Contains(nextSpawnY))
					{
						FTransform t(FRotator(0, 180, 0), floorMap.FindChecked(nextSpawnY), FVector(1, 1, 1));
						SpawnISM(t, Walls);
					}
				}
			}
		}
	}
}

void ARoomManager::PlaceAtCoOrdinate(int x, int y, TSubclassOf<AActor> actor)
{
	//convert the given x and y to a string for the map key
	FString coordinates = FString::FromInt(x) + "," + FString::FromInt(y);
	//check for invalid coordinates
	if (floorMap.Contains(coordinates))
	{
		//spawn the actor
		Spawn(floorMap.FindChecked(coordinates), actor);
	}
}

void ARoomManager::TurretRing()
{
	ALevelManager* lm = new ALevelManager();

	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}
	//assign the room type
	assignedRoomType = RoomType::RT_TURRET;
	//intiialise the turret actor
	Turret = lm->_Turret;
	
	TArray<FString> positions;
	FString arr[] = { "13,4", "9,8", "17,8", "5,12", "21,12", "9,16", "17,16", "13,20" };
	positions.Append(arr, ARRAY_COUNT(arr));
	
	for (int i = 0; i < positions.Num(); i++)
	{
		FVector nextPosition = floorMap.FindChecked(positions[i]);
		Spawn(nextPosition, Turret);
	}
}

//returns array of neighboring nodes to a given start point
TMap<FString, FVector> ARoomManager::GetNodeNeighbors(FVector start)
{
	//use a Tmap instead :D
	TMap<FString, FVector> neighborMap;

	ALevelManager* lm = new ALevelManager();
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}
	//the position of the next neighbor to be checked
	FVector nextNeighborPosition = FVector(0, 0, 0);

	//+ x axis check
	nextNeighborPosition = FVector((start.X + lm->unitSize), start.Y, start.Z);
	///check for walls
	if (!wallCoords.Contains(FVector(nextNeighborPosition.X, nextNeighborPosition.Y, (nextNeighborPosition.Z + (lm->unitSize * 0.5)) )) && floorCoords.Contains(nextNeighborPosition))
	{
		neighborMap.Add("posX", nextNeighborPosition);
	}
	//- x axis check
	nextNeighborPosition = FVector((start.X - lm->unitSize), start.Y, start.Z);
	///check for walls
	if (!wallCoords.Contains(FVector(nextNeighborPosition.X, nextNeighborPosition.Y, (nextNeighborPosition.Z + (lm->unitSize * 0.5)))) && floorCoords.Contains(nextNeighborPosition))
	{
		neighborMap.Add("negX", nextNeighborPosition);
	}
	//+ y axis check
	nextNeighborPosition = FVector(start.X, (start.Y + lm->unitSize), start.Z);
	///check for walls
	if (!wallCoords.Contains(FVector(nextNeighborPosition.X, nextNeighborPosition.Y, (nextNeighborPosition.Z + (lm->unitSize * 0.5)))) && floorCoords.Contains(nextNeighborPosition))
	{
		neighborMap.Add("posY", nextNeighborPosition);
	}
	//-y axis check
	nextNeighborPosition = FVector(start.X, (start.Y - lm->unitSize), start.Z);
	//check for walls
	if (!wallCoords.Contains(FVector(nextNeighborPosition.X, nextNeighborPosition.Y, (nextNeighborPosition.Z + (lm->unitSize * 0.5)))) && floorCoords.Contains(nextNeighborPosition))
	{
		neighborMap.Add("negY", nextNeighborPosition);
	}

	return neighborMap;
}

//Please note that this function gets the last level manager in the scene so please ensure there is only one level manager at any one time.
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();	

	ALevelManager* lm = new ALevelManager();
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}
	

	Floors = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Floors->RegisterComponent();
	Floors->SetStaticMesh(lm->Floor1);
	Floors->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Floors);

	Walls = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Walls->RegisterComponent();
	Walls->SetStaticMesh(lm->Wall);
	Walls->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Walls);
	
	GenerateRoom(lm->roomXSize, lm->roomYSize);

	//if this is not the start room (parameters can be changed)
	if (this->GetActorLocation() != FVector(0, 0, 0))
	{
		int percentage = rand() % (100 + 1);
		//decide whether this room has turrets (50% chance for now)
		if (percentage < 25)
		{
			if (lm->turretRooms < 3)
			{
				TurretRing();
				lm->turretRooms++;
			}
			else
			{
				assignedRoomType = RoomType::RT_GENERIC;
				lm->genericRooms++;
			}
		}
		else if (percentage > 25 && percentage < 50)
		{
			if (lm->weaponRooms < 3)
			{
				int w = rand() % 20 + 5;
				int h = rand() % 20 + 5;
				DrawInteriorRoom(20, 10, w, h);
				lm->weaponRooms++;
			}
			else
			{
				assignedRoomType = RoomType::RT_GENERIC;
				lm->genericRooms++;
			}
		}
		else if (percentage > 50 && percentage < 100)
		{
			if (lm->enemyRooms < 10)
			{
				assignedRoomType = RoomType::RT_ENEMY;
				lm->enemyRooms++;
			}
			else
			{
				assignedRoomType = RoomType::RT_GENERIC;
				lm->genericRooms++;
			}
		}
	}
	else
	{
		//this is the start room
		assignedRoomType = RoomType::RT_START;
	}
	
}

// Called every frame
void ARoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	time += DeltaTime;
	if (time > 10) {
		this->SetActorTransform(FTransform(FVector(0, 0, 5)));
		time = 0;
	}
}



