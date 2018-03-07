

#include "BSP.h"

//https://stackoverflow.com/questions/4997642/simple-example-of-bsp-dungeon-generation


ABSP::ABSP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
// Called when the game starts or when spawned
void ABSP::BeginPlay()
{
	Super::BeginPlay();

	Floors = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Floors->RegisterComponent();
	Floors->SetStaticMesh(Floor);
	Floors->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Floors);

	Walls = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Walls->RegisterComponent();
	Walls->SetStaticMesh(Wall);
	Walls->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Walls);
	
	CreateLevel();
	//DrawMap();
	AddRoomManagers();
	DrawRooms();
	AddDoors();
}


// Called every frame
void ABSP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABSP::CreateLevel() {
	FChunk master = { -2, -2, -2, -2, -2, FVector(levelWidth*unitSize / 2, levelHeight*unitSize/2, 0), levelWidth, levelHeight, 0, 0, false };
	chunks.Add(master);
	Split(0);
	
	for (int i = 1; i < subDivisions; i += 2) {
			Split(i);
			Split(i+1);
	}
}

//splits are always draw left and up respectivly
bool ABSP::Split(int i) {
	if (i > chunks.Num() - 1) {
		return false;
	}
	int r = 0;
	if (!chunks[i].horizontal && chunks[i].chunkWidth >= minWidth*2) {
		//r = FMath::RandRange(minWidth, chunks[i].chunkWidth - minWidth);
		r = 2 * (rand() % (minWidth / 2)) + ((chunks[i].chunkWidth - minWidth) / 2);

		FChunk rightChild = { i, -2, -2, -2, -2, 
			FVector((chunks[i].chunkCenter.X - ((chunks[i].chunkWidth/2)*unitSize) + (r/2)*unitSize), chunks[i].chunkCenter.Y, 0),
			r, chunks[i].chunkHeight, -2, -2, true };

		FChunk leftChild = { i, -2, -2, -2, -2,
			FVector(rightChild.chunkCenter.X+(chunks[i].chunkWidth/2)*unitSize, chunks[i].chunkCenter.Y, 0),
			chunks[i].chunkWidth - r, chunks[i].chunkHeight, -2, -2, true };


		chunks.Add(rightChild);
		chunks.Add(leftChild);

		chunks[i].RightChild = i+1;
		chunks[i].LeftChild = i+2;
		chunks[i].splitX = r;
		chunks[i].splitY = chunks[i].chunkHeight / 2;
		return true;
	}
	else if (chunks[i].horizontal && chunks[i].chunkHeight >= minHeight * 2) {
		//r = FMath::RandRange(minHeight, chunks[i].chunkHeight - minHeight);
		r = 2 * (rand() % (minHeight / 2)) + ((chunks[i].chunkHeight - minHeight) / 2);

		FChunk bottomChild = { i, -2, -2, -2, -2,
			FVector(chunks[i].chunkCenter.X, (chunks[i].chunkCenter.Y - ((chunks[i].chunkHeight/2)*unitSize) + (r / 2)*unitSize), 0),
			chunks[i].chunkWidth, r, -2, -2, false };

		FChunk topChild = { i, -2, -2, -2, -2,
			FVector(chunks[i].chunkCenter.X, bottomChild.chunkCenter.Y + ((chunks[i].chunkHeight / 2)*unitSize), 0),
			chunks[i].chunkWidth, chunks[i].chunkHeight - r, -2, -2, false };


		chunks.Add(bottomChild);
		chunks.Add(topChild);

		chunks[i].BottomChild = i+1;
		chunks[i].TopChild = i+2;
		chunks[i].splitX = chunks[i].chunkWidth / 2;
		chunks[i].splitY = r;
		return true;
	}
	else {
		return false;
	}
}

void ABSP::DrawMap() {
	if (chunks.Num() != 0) {
		FVector br(chunks[0].chunkCenter.X - ((chunks[0].chunkWidth*unitSize) / 2), chunks[0].chunkCenter.Y - ((chunks[0].chunkHeight*unitSize) / 2), 0);
		//Draw floor
		for (int y = 0; y < chunks[0].chunkHeight; y++) {
			for (int x = 0; x < chunks[0].chunkWidth; x++) {
				FTransform ft(FRotator(0, 0, 0), FVector(br.X + (x*unitSize), br.Y + (y*unitSize), 0), FVector(1, 1, 1));
				SpawnISM(ft, Floors);
			}
		}
		for (int i = chunks.Num() - 1; i >= 0; i--) {
			FVector br(chunks[i].chunkCenter.X - ((chunks[i].chunkWidth*unitSize) / 2), chunks[i].chunkCenter.Y - ((chunks[i].chunkHeight*unitSize) / 2), 0);
			
			//draw walls
			//right wall up
			for (int j = 0; j < chunks[i].chunkHeight; j++) {
				FTransform wt(FRotator(0, 0, 0), FVector(br.X, br.Y + (j*unitSize), 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//top wall left
			for (int j = 0; j < chunks[i].chunkWidth; j++) {
				FTransform wt(FRotator(0, 0, 0), FVector(br.X + (j*unitSize), br.Y + (chunks[i].chunkHeight*unitSize), 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//left wall down
			for (int j = chunks[i].chunkHeight; j > 0; j--) {
				FTransform wt(FRotator(0, 0, 0), FVector(br.X + (chunks[i].chunkWidth*unitSize), br.Y + (j*unitSize), 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//bottom wall right
			for (int j = chunks[i].chunkWidth; j > 0; j--) {
				FTransform wt(FRotator(0, 0, 0), FVector(br.X + (j*unitSize), br.Y, 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

		}
	}
}

void ABSP::AddRoomManagers() {
	TArray<FChunk> leaf;

	//get all leaf nodes from chunks. These are the rooms that have not been sub divided. 
	for (int i = 0; i < chunks.Num(); i++) {
		if (chunks[i].BottomChild == -2 && chunks[i].TopChild == -2 && chunks[i].RightChild == -2 && chunks[i].LeftChild == -2) {
			leaf.Add(chunks[i]);
		}
	}

	for (int i = 0; i < leaf.Num(); i++) {
		rms.Add(SpawnRM(leaf[i].chunkCenter));
		rms[rms.Num() - 1]->location = leaf[i].chunkCenter;
		rms[rms.Num() - 1]->width = leaf[i].chunkWidth;
		rms[rms.Num() - 1]->height = leaf[i].chunkHeight;
	}
}

void ABSP::DrawRooms() {
	for (int i = 0; i < rms.Num(); i++) {
		rms[i]->DrawRoom();
	}
}

ABSPRoomManager* ABSP::SpawnRM(FVector location)
{
	if (rm)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			return world->SpawnActor<ABSPRoomManager>(rm, location, FRotator::ZeroRotator, spawnParams);
		}
	}
	return nullptr;
}

void ABSP::AddDoors() {
	/*TArray<FChunk> leafTemp;
	//get all leaf nodes from chunks. These are the rooms that have not been sub divided. 
	for (int i = 0; i < chunks.Num(); i++) {
		if (chunks[i].BottomChild == -2 && chunks[i].TopChild == -2 && chunks[i].RightChild == -2 && chunks[i].LeftChild == -2) {
			leafTemp.Add(chunks[i]);
		}
	}

	for (int i = 0; i < leafTemp.Num(); i++) {
		FChunk parent = chunks[leafTemp[i].parent];
		if (parent.BottomChild != 0 && parent.BottomChild != 0) {
			FVector loc(parent.chunkCenter.X, chunks[parent.BottomChild].chunkCenter.Y + chunks[parent.BottomChild].chunkHeight / 2, 0);
			Spawn(loc, Door);
			//leafTemp.RemoveAt(i);
			//leafTemp.RemoveAt(i+1);
		}
		else if (parent.LeftChild != 0 && parent.RightChild != 0) {
			FVector loc(chunks[parent.RightChild].chunkCenter.X + chunks[parent.RightChild].chunkWidth/2, parent.chunkCenter.Y, 0);
			Spawn(loc, Door);
			//leafTemp.RemoveAt(i);
			//leafTemp.RemoveAt(i+1);
		}

		FChunk pparent = chunks[parent.parent];
		if (pparent.BottomChild != 0 && pparent.TopChild != 0) {
			FVector loc(pparent.chunkCenter.X, chunks[pparent.BottomChild].chunkCenter.Y + chunks[pparent.BottomChild].chunkHeight / 2, 0);
			Spawn(loc, Door);
		}
		else if (pparent.LeftChild != 0 && pparent.RightChild != 0){
			FVector loc(chunks[pparent.RightChild].chunkCenter.X + chunks[pparent.RightChild].chunkWidth / 2, pparent.chunkCenter.Y, 0);
			Spawn(loc, Door);
		}
		
	}*/

	for (int i = 0; i < chunks.Num(); i++) {

		if (chunks[i].BottomChild != -2 && chunks[i].TopChild != -2) {
			FVector loc(chunks[i].chunkCenter.X, (chunks[i].chunkCenter.Y - (chunks[i].chunkHeight*unitSize) / 2) + chunks[i].splitY*unitSize, 0);

			int bottomSplit = chunks[chunks[i].BottomChild].splitX;
			int topSplit = chunks[chunks[i].TopChild].splitX;

			if (((chunks[i].chunkWidth - bottomSplit) + (chunks[i].chunkWidth - topSplit)) > bottomSplit + topSplit) {
				loc.X = chunks[i].chunkCenter.X + (chunks[i].chunkWidth*unitSize / 2) - ((minWidth-2)*unitSize / 2);
			}
			else {
				loc.X = chunks[i].chunkCenter.X - (chunks[i].chunkWidth*unitSize / 2) + ((minWidth-2)*unitSize / 2);
			}

			if ((int)loc.X % unitSize != 0) {
				loc.X += unitSize / 2;
			}

			if ((int)loc.Y % unitSize != 0) {
				loc.Y += unitSize / 2;
			}
			
			int doorNegX = loc.X - (unitSize);
			int doorNegY = loc.Y - (unitSize);

			for (int j = 0; j < rms.Num(); j++) {
				for (int k = 0; k < rms[j]->Walls->GetInstanceCount(); k++) {
					FTransform wallLoc(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(0, 0, 0));
					rms[j]->Walls->GetInstanceTransform(k, wallLoc, true);
					if (wallLoc.GetLocation() == FVector(loc.X, loc.Y, 0) || wallLoc.GetLocation() == FVector(doorNegX, loc.Y, 0) || wallLoc.GetLocation() == FVector(doorNegX, doorNegY, 0) || wallLoc.GetLocation() == FVector(loc.X, doorNegY, 0)) {
						wallLoc.SetLocation(FVector(loc.X, loc.Y, -300));
						wallLoc.SetScale3D(FVector(0, 0, 0));
						rms[j]->Walls->UpdateInstanceTransform(k, wallLoc, true, false, true);
					}
				}
			}

			loc.X = loc.X - (unitSize / 2);
			loc.Y = loc.Y - (unitSize / 2);

			FTransform trans(FRotator(0, 90, 0), loc, FVector(1, 1, 1));
			SpawnTransform(trans, Door);
		}
		else if (chunks[i].LeftChild != -2 && chunks[i].RightChild != -2) {
			FVector loc((chunks[i].chunkCenter.X - (chunks[i].chunkWidth*unitSize) / 2) + chunks[i].splitX*unitSize, chunks[i].chunkCenter.Y, 0);

			int rightSplit = chunks[chunks[i].RightChild].splitY;
			int leftSplit = chunks[chunks[i].LeftChild].splitY;

			if (((chunks[i].chunkHeight - rightSplit) + (chunks[i].chunkHeight - leftSplit)) > rightSplit + leftSplit) {
				loc.Y = chunks[i].chunkCenter.Y + (chunks[i].chunkHeight*unitSize / 2) - ((minHeight-2)*unitSize / 2);
			}
			else {
				loc.Y = chunks[i].chunkCenter.Y - (chunks[i].chunkHeight*unitSize / 2) + ((minHeight-2)*unitSize / 2);
			}

			if ((int)loc.X % unitSize != 0) {
				loc.X += unitSize / 2;
			}

			if ((int)loc.Y % unitSize != 0) {
				loc.Y += unitSize / 2;
			}
			
			int doorNegX = loc.X - (unitSize);
			int doorNegY = loc.Y - (unitSize);

			for (int j = 0; j < rms.Num(); j++) {
				for (int k = 0; k < rms[j]->Walls->GetInstanceCount(); k++) {
					FTransform wallLoc(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(0, 0, 0));
					rms[j]->Walls->GetInstanceTransform(k, wallLoc, true);
					if (wallLoc.GetLocation() == FVector(loc.X, loc.Y, 0) || wallLoc.GetLocation() == FVector(doorNegX, loc.Y, 0) || wallLoc.GetLocation() == FVector(doorNegX, doorNegY, 0) || wallLoc.GetLocation() == FVector(loc.X, doorNegY, 0)) {
						wallLoc.SetLocation(FVector(loc.X, loc.Y, -300));
						wallLoc.SetScale3D(FVector(0, 0, 0));
						rms[j]->Walls->UpdateInstanceTransform(k, wallLoc, true, false, true);
					}
				}
			}

			loc.X = loc.X - (unitSize / 2);
			loc.Y = loc.Y - (unitSize / 2);

			FTransform trans(FRotator(0, 0, 0), loc, FVector(1, 1, 1));
			SpawnTransform(trans, Door);
		}
	}

	
}

void ABSP::Spawn(FVector location, TSubclassOf<AActor> actor)
{

	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		world->SpawnActor<AActor>(actor, location, FRotator::ZeroRotator, spawnParams);
	}

}

void ABSP::SpawnTransform(FTransform position, TSubclassOf<AActor> actor)
{
	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		world->SpawnActor<AActor>(actor, position, spawnParams);
	}
}

