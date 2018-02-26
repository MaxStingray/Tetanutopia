

#include "BSP.h"

//https://stackoverflow.com/questions/4997642/simple-example-of-bsp-dungeon-generation

void * ABSP::operator new(size_t size)
{
	return nullptr;
}

ABSP::ABSP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

ABSP::ABSP(int t, int l, int w, int h) {
	top = t;
	left = l;
	width = w;
	height = h;
}



// Called when the game starts or when spawned
void ABSP::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABSP::split() {
	if (leftChild != nullptr) //if already split, bail out
		return false;
	int horizontal = rand() % 1+1; //direction of split
	int max = (horizontal ? height : width) - MIN_SIZE; //maximum height/width we can split off
	if (max <= MIN_SIZE) // area too small to split, bail out
		return false;
	int split = rand() % max+1; // generate split point 
	if (split < MIN_SIZE)  // adjust split point so there's at least MIN_SIZE in both partitions
		split = MIN_SIZE;
	if (horizontal) { //populate child areas
		leftChild = new ABSP(top, left, split, width);
		rightChild = new ABSP(top + split, left, height - split, width);
	}
	else {
		leftChild = new ABSP(top, left, height, split);
		rightChild = new ABSP(top, left + split, height, width - split);
	}
	return true; //split successful
}

void ABSP::generateDungeon() {
	if (leftChild != nullptr) { //if current are has child areas, propagate the call
		leftChild->generateDungeon();
		rightChild->generateDungeon();
	}
	else { // if leaf node, create a dungeon within the minimum size constraints
		int dungeonTop = (height - MIN_SIZE <= 0) ? 0 : rand() % height - MIN_SIZE + 1;
		int dungeonLeft = (width - MIN_SIZE <= 0) ? 0 : rand() % width - MIN_SIZE + 1;
		int dungeonHeight = FMath::Max3((rand() % height - dungeonTop + 1), MIN_SIZE, 0);
		int dungeonWidth = FMath::Max3((rand() % width - dungeonLeft + 1), MIN_SIZE, 0);
		dungeon = new ABSP(top + dungeonTop, left + dungeonLeft, dungeonHeight, dungeonWidth);
	}
}



// Called every frame
void ABSP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

