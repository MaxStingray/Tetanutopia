
#include "BSPRoomManager.h"
#include "EngineUtils.h"

// Sets default values
ABSPRoomManager::ABSPRoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABSPRoomManager::BeginPlay()
{
	Super::BeginPlay();

	ABSP* bsp = nullptr;

	for (TActorIterator<ABSP> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		bsp = *ActorItr;
	}

	
	Floors = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Floors->RegisterComponent();
	Floors->SetStaticMesh(bsp->Floor);
	Floors->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Floors);

	Walls = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
	Walls->RegisterComponent();
	Walls->SetStaticMesh(bsp->Wall);
	Walls->SetFlags(RF_Transactional);
	this->AddInstanceComponent(Walls);

	for (int i = 0; i < width * height; i++) {
		Tiles.Add(0);
	}

}

// Called every frame
void ABSPRoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABSPRoomManager::DrawRoom() {

	ABSP* bsp = nullptr;

	for (TActorIterator<ABSP> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		bsp = *ActorItr;
	}

	FVector br(location.X - (width/2*bsp->unitSize), location.Y - (height/2*bsp->unitSize), 0);
	//draw floor
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			FTransform ft(FRotator(0, 0, 0), FVector(br.X + (x*bsp->unitSize), br.Y + (y*bsp->unitSize), 0), FVector(1, 1, 1));
			bsp->SpawnISM(ft, Floors);
			//SetAvb(x, y, (int)Avb::EMPTY);
		}
	}

	//draw walls
	//right wall up
	for (int j = 0; j < height; j++) {
		FTransform wt(FRotator(0, 0, 0), FVector(br.X, br.Y + (j*bsp->unitSize), 0), FVector(1, 1, 1));
		bsp->SpawnISM(wt, Walls);
		//SetAvb(wt.GetLocation().X / bsp->unitSize, wt.GetLocation().Y / bsp->unitSize, (int)Avb::WALL);
	}

	//top wall left
	for (int j = 0; j < width; j++) {
		FTransform wt(FRotator(0, 0, 0), FVector(br.X + (j*bsp->unitSize), br.Y + (height*bsp->unitSize) - bsp->unitSize, 0), FVector(1, 1, 1));
		bsp->SpawnISM(wt, Walls);
		//SetAvb(wt.GetLocation().X / bsp->unitSize, wt.GetLocation().Y / bsp->unitSize, (int)Avb::WALL);
	}

	//left wall down
	for (int j = height - 1; j > 0; j--) {
		FTransform wt(FRotator(0, 0, 0), FVector(br.X + (width*bsp->unitSize) - bsp->unitSize, br.Y + (j*bsp->unitSize), 0), FVector(1, 1, 1));
		bsp->SpawnISM(wt, Walls);
		//SetAvb(wt.GetLocation().X / bsp->unitSize, wt.GetLocation().Y / bsp->unitSize, (int)Avb::WALL);
	}

	//bottom wall right
	for (int j = width - 1; j > 0; j--) {
		FTransform wt(FRotator(0, 0, 0), FVector(br.X + (j*bsp->unitSize), br.Y, 0), FVector(1, 1, 1));
		bsp->SpawnISM(wt, Walls);
		//SetAvb(wt.GetLocation().X / bsp->unitSize, wt.GetLocation().Y / bsp->unitSize, (int)Avb::WALL);
	}
}

void ABSPRoomManager::PlaceProps() {

}

void ABSPRoomManager::SetAvb(int row, int col, int value)
{
	Tiles[width * row + col] = value;
}

int ABSPRoomManager::GetAvb(int row, int col)
{
	return Tiles[width * row + col];
}

