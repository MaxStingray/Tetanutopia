

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
	DrawMap();
}


// Called every frame
void ABSP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABSP::CreateLevel() {
	FChunk master = { nullptr, nullptr, nullptr, nullptr, nullptr, FVector(levelWidth*unitSize / 2, levelHeight*unitSize/2, 0), levelWidth, levelHeight, 0, 0, false };
	chunks.Add(master);
	Split(0);
	
	for (int i = 1; i < subDivisions; i += 2) {
			Split(i);
			Split(i+1);
	}
}

//splits are always draw left and up respectivly
bool ABSP::Split(int i) {

	int r = 0;
	if (!chunks[i].horizontal && chunks[i].chunkWidth >= minWidth*2) {
		r = FMath::RandRange(minWidth, chunks[i].chunkWidth - minWidth);

		FChunk rightChild = { &chunks[i], nullptr, nullptr, nullptr, nullptr, 
			FVector((chunks[i].chunkCenter.X - ((chunks[i].chunkWidth/2)*unitSize) + (r/2)*unitSize), chunks[i].chunkCenter.Y, 0),
			r, chunks[i].chunkHeight, 0, 0, false };

		FChunk leftChild = { &chunks[i], nullptr, nullptr, nullptr, nullptr, 
			FVector(rightChild.chunkCenter.X+(chunks[i].chunkWidth/2)*unitSize, chunks[i].chunkCenter.Y, 0),
			chunks[i].chunkWidth - r, chunks[i].chunkHeight, 0, 0, false };


		chunks.Add(rightChild);
		chunks.Add(leftChild);

		chunks[i].RightChild = &rightChild;
		chunks[i].LeftChild = &leftChild;
		chunks[i].splitX = r;
		chunks[i].splitY = chunks[i].chunkHeight / 2;
		return true;
	}
	else if (chunks[i].horizontal && chunks[i].chunkHeight >= minHeight * 2) {
		r = FMath::RandRange(minHeight, chunks[i].chunkHeight - minHeight);
		
		FChunk bottomChild = { &chunks[i], nullptr, nullptr, nullptr, nullptr,
			FVector(chunks[i].chunkCenter.X, (chunks[i].chunkCenter.Y - ((chunks[i].chunkHeight/2)*unitSize) + (r / 2)*unitSize), 0),
			chunks[i].chunkWidth, r, 0, 0, true };

		FChunk topChild = { &chunks[i], nullptr, nullptr, nullptr, nullptr,
			FVector(chunks[i].chunkCenter.X, bottomChild.chunkCenter.Y + ((chunks[i].chunkHeight / 2)*unitSize), 0),
			chunks[i].chunkWidth, chunks[i].chunkHeight - r, 0, 0, true };


		chunks.Add(bottomChild);
		chunks.Add(topChild);

		chunks[i].BottomChild = &bottomChild;
		chunks[i].TopChild = &topChild;
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
		for (int i = chunks.Num() - 1; i >= 0; i--) {
			//Remember this is tiles! it has not been * by unit size.
			FVector br(chunks[i].chunkCenter.X - (chunks[i].chunkWidth / 2), chunks[i].chunkCenter.Y - (chunks[i].chunkHeight / 2), 0);
			//Draw floor
			for (int y = 0; y < chunks[i].chunkHeight; y++) {
				for (int x = 0; x < chunks[i].chunkWidth; x++) {
					FTransform ft(FRotator(0, 0, 0), FVector((br.X + x)*unitSize, (br.Y + y)*unitSize, 0), FVector(1,1,1));
					SpawnISM(ft, Floors);
				}
			}

			//draw walls
			//right wall up
			for (int j = 0; j < chunks[i].chunkHeight; j++) {
				FTransform wt(FRotator(0, 0, 0), FVector(br.X*unitSize, (br.Y + j)*unitSize, 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//top wall left
			for (int j = 0; j < chunks[i].chunkWidth; j++) {
				FTransform wt(FRotator(0, 0, 0), FVector((br.X + j)*unitSize, (br.Y + chunks[i].chunkHeight)*unitSize, 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//left wall down
			for (int j = chunks[i].chunkHeight; j > 0; j--) {
				FTransform wt(FRotator(0, 0, 0), FVector((br.X + chunks[i].chunkWidth)*unitSize, (br.Y + j)*unitSize, 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

			//bottom wall right
			for (int j = chunks[i].chunkWidth; j > 0; j--) {
				FTransform wt(FRotator(0, 0, 0), FVector((br.X + j)*unitSize, br.Y*unitSize, 0), FVector(1, 1, 1));
				SpawnISM(wt, Walls);
			}

		}
	}
}

