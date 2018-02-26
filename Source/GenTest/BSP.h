

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSP.generated.h"

UCLASS()
class GENTEST_API ABSP : public AActor
{
	GENERATED_BODY()

public:
	ABSP();
	ABSP(int t, int l, int w, int h);


	static const int MIN_SIZE = 5;
	int top, left, width, height;
	ABSP* leftChild;
	ABSP* rightChild;
	ABSP* dungeon;

	// Sets default values for this actor's properties



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool split();
	void * operator new(size_t size);
	void generateDungeon();
	
	
};