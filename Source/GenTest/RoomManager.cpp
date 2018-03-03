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
void ARoomManager::SpawnTransform(FTransform position, TSubclassOf<AActor> actor)
{
	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		world->SpawnActor<AActor>(actor, position.GetLocation(), FRotator(position.GetRotation()), spawnParams);
	}
}
//can probably refactor SpawnTurrets and SpawnEnemies to be one method. This is fine for now
void ARoomManager::SpawnEnemies()
{

	//assign the room type
	assignedRoomType = RoomType::RT_ENEMY;
	ALevelManager* lm = new ALevelManager();
	
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}


	Enemy = lm->_Enemy;
	TArray<FString> positions;
	int percentage = rand() % 100 + 1;

	if (percentage < 50)
	{
		FString arr[] = { "13,4", "9,8", "17,8", "5,12", "21,12", "9,16", "17,16", "13,20" };
		positions.Append(arr, ARRAY_COUNT(arr));
	}
	else if (percentage > 50)
	{
		FString arr[] = { "2,2", "24,24", "24,2", "2,24" };
		positions.Append(arr, ARRAY_COUNT(arr));
	}

	for (int i = 0; i < positions.Num(); i++)
	{
		FVector nextPosition = floorMap.FindChecked(positions[i]);
		Spawn(nextPosition, Enemy);
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
			//get local coordinates from _x and _y and make a string
			//string is the key value to access world coordinates from the TMap
			FString locationString = (FString::FromInt(_x) + "," + FString::FromInt(_y));
			//add coordinates to collection
			floorMap.Add(locationString, FVector(x, y, 0));
			//see if we should place a prop
			PlaceProps(locationString);
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
	ALevelManager* lm = new ALevelManager();

	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}

	int endX = startX - width;//might need to be +
	int endY = startY + height;//also might be wrong

	//interior room center
	int intCenterX = width / 2;
	int intCenterY = height / 2;
	FString intCoord = FString::FromInt(intCenterX) + "," + FString::FromInt(intCenterY);
	FVector intRoomCenter = floorMap.FindChecked(intCoord);

	//exterior room center
	int extRoomCenterX = lm->roomXSize / 2;
	int extRoomCenterY = lm->roomYSize / 2;
	FString extCoord = FString::FromInt(extRoomCenterX) + "," + FString::FromInt(extRoomCenterY);
	FVector extRoomCenter = floorMap.FindChecked(extCoord);

	//find the distance between them
	float distX = extRoomCenter.X - intRoomCenter.X;
	float distY = extRoomCenter.Y - intRoomCenter.Y;
	FVector dist = FVector(distX, distY, 0);
	DebugDistance = dist;

	//assign room type
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

	//check which distance is greater
	if (FMath::Abs(distX) > FMath::Abs(distY))
	{
		//
		if (distX < 0)
		{
			//spawn door on north wall on positive x axis
			FTransform debug(FRotator(90), FVector(0, 0, 0), FVector(0, 0, 0));
			Walls->GetInstanceTransform((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + height + (width / 2), debug);
			DebugDistance = debug.GetLocation();
			Walls->RemoveInstance((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + height + (width / 2));
		}
		else
		{
			//spawn on south wall
			FTransform debug(FRotator(90), FVector(0, 0, 0), FVector(0, 0, 0));
			Walls->GetInstanceTransform((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + height + (width / 2), debug);
			DebugDistance = debug.GetLocation();
			Walls->RemoveInstance((lm->roomXSize * 2) + (lm->roomYSize * 2) + width / 2);
		}
	}
	else
	{
		if (distY < 0)
		{
			//spawn on west wall
			FTransform debug(FRotator(90), FVector(0, 0, 0), FVector(0, 0, 0));
			Walls->GetInstanceTransform((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + height + (width / 2), debug);
			DebugDistance = debug.GetLocation();
			Walls->RemoveInstance((lm->roomXSize * 2) + (lm->roomYSize * 2) + (width + height) + width + (height / 2));
		}
		else
		{
			//spawn on east wall
			FTransform debug(FRotator(90), FVector(0, 0, 0), FVector(0, 0, 0));
			Walls->GetInstanceTransform((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + height + (width / 2), debug);
			DebugDistance = debug.GetLocation();
			Walls->RemoveInstance((lm->roomXSize * 2) + (lm->roomYSize * 2) + width + (height / 2));
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
	int percentage = rand() % 100 + 1;

	if (percentage < 50)
	{
		FString arr[] = { "13,4", "9,8", "17,8", "5,12", "21,12", "9,16", "17,16", "13,20" };
		positions.Append(arr, ARRAY_COUNT(arr));
	}
	else if (percentage > 50)
	{
		FString arr[] = { "2,2", "24,24", "24,2", "2,24" };
		positions.Append(arr, ARRAY_COUNT(arr));
	}
	
	for (int i = 0; i < positions.Num(); i++)
	{
		FVector nextPosition = floorMap.FindChecked(positions[i]);
		Spawn(nextPosition, Turret);
	}
}
//place a prop if condition is met
void ARoomManager::PlaceProps(FString location)
{
	//get the level manager.. there has to be a less shit way to do this
	ALevelManager* lm = new ALevelManager();
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		lm = *ActorItr;
	}
	//give props a percentage chance to spawn per tile
	int percentage = rand() % 100 + 1;
	if (percentage == 1)
	{
		if (lm->props.Num() != 0)
		{
			//select the prop
			int whichProp = rand() % lm->props.Num();//remove brackets if necessary
			TSubclassOf<AActor> actor = lm->props[whichProp];
			//spawn it
			Spawn(floorMap.FindChecked(location), actor);
		}
	}

	//TODO: Make this less random. Take into account wall positions and record placement of other objects
}

//returns array of neighboring nodes to a given start point
TMap<FString, FVector> ARoomManager::GetNodeNeighbors(FString startLocation, int numTiles)//change to location string, add number of tiles
{
	/*TODO: give getnodeNeighbors a number of tiles in each direction to check
	only one press and one fire pit per room
	place barrels and pallets close to walls

	then roomgen is DONE*/


	//split the given string into two integers
	FString _x;
	FString _y;
	startLocation.Split(TEXT(","), &_x, &_y);
	int startX = FCString::Atoi(*_x);
	int startY = FCString::Atoi(*_y);
	//no, I can't believe this is how it works either
	//return a TMap of neighboring tiles
	TMap<FString, FVector> neighborMap;
	//create the start point key from the given parameters
	FString startKey = FString::FromInt(startX) + "," + FString::FromInt(startY);
	//get the start vector from the floormap using the created key
	FVector start = floorMap.FindChecked(startKey);
	ALevelManager* lm = new ALevelManager();
	for (TActorIterator<ALevelManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		lm = *ActorItr;
	}

	//+ x axis check
	//get the key of the next neighbor to check
	FString nextNeighborKey = FString::FromInt(startX + 1) + "," + FString::FromInt(startY);
	FVector wallCheck = FVector(floorMap.FindChecked(nextNeighborKey).X, floorMap.FindChecked(nextNeighborKey).Y, floorMap.FindChecked(nextNeighborKey).Z + lm->unitSize);
	///check for walls
	if (!wallCoords.Contains(wallCheck) && floorMap.Contains(nextNeighborKey))
	{
		neighborMap.Add("posX", floorMap.FindChecked(nextNeighborKey));
	}
	//- x axis check
	nextNeighborKey = FString::FromInt(startX - 1) + "," + FString::FromInt(startY);
	wallCheck = FVector(floorMap.FindChecked(nextNeighborKey).X, floorMap.FindChecked(nextNeighborKey).Y, floorMap.FindChecked(nextNeighborKey).Z + lm->unitSize);
	///check for walls
	if (!wallCoords.Contains(wallCheck) && floorMap.Contains(nextNeighborKey))
	{
		neighborMap.Add("negX", floorMap.FindChecked(nextNeighborKey));
	}
	//+ y axis check
	nextNeighborKey = FString::FromInt(startX) + "," + FString::FromInt(startY + 1);
	wallCheck = FVector(floorMap.FindChecked(nextNeighborKey).X, floorMap.FindChecked(nextNeighborKey).Y, floorMap.FindChecked(nextNeighborKey).Z + lm->unitSize);
	///check for walls
	if (!wallCoords.Contains(wallCheck) && floorMap.Contains(nextNeighborKey))
	{
		neighborMap.Add("posY", floorMap.FindChecked(nextNeighborKey));
	}
	//-y axis check
	nextNeighborKey = FString::FromInt(startX) + "," + FString::FromInt(startY - 1);
	wallCheck = FVector(floorMap.FindChecked(nextNeighborKey).X, floorMap.FindChecked(nextNeighborKey).Y, floorMap.FindChecked(nextNeighborKey).Z + lm->unitSize);
	///check for walls
	if (!wallCoords.Contains(wallCheck) && floorMap.Contains(nextNeighborKey))
	{
		neighborMap.Add("negY", floorMap.FindChecked(nextNeighborKey));
	}

	return neighborMap;
}

//Please note that this function gets the last level manager in the scene so please ensure there is only one level manager at any one time.
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();	
	playerLocation = FVector(0, 0, 0);
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
				SpawnEnemies();
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
		this->SetActorTransform(FTransform(FVector(0, 0, 0)));
		time = 0;
	}
	try
	{
		//TODO: Get player location here somehow
		//playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	}
	catch (const std::exception&)
	{
		playerLocation = FVector(0, 0, 0);
	}
}



