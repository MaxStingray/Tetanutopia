// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomManager.h"
#include "LevelManager.generated.h"
/*There always has to be one level manager in eash scene/level. Having more than one might also cause a problem*/
UCLASS()
class GENTEST_API ALevelManager : public AActor
{
	GENERATED_BODY()

private:
	

public:	
	// Sets default values for this actor's properties
	ALevelManager();

	//room counters, increment in RoomManager
	UPROPERTY(EditAnywhere)
		int turretRooms = 0;
	UPROPERTY(EditAnywhere)
		int enemyRooms = 0;
	UPROPERTY(EditAnywhere)
		int weaponRooms = 0;
	UPROPERTY(EditAnywhere)
		int genericRooms = 0;
	void * operator new(size_t size);
	static const int roomsX = 4;
	static const int roomsY = 4;
	int roomXSize = 26;
	int roomYSize = 26;
	int unitSize = 100;
	ARoomManager* Rooms[roomsX][roomsY];
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ARoomManager> rm;
	

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> _Turret;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Floor1;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Floor2;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Wall;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateLevel();
	ARoomManager* Spawn(FVector location);
	void CreatePath();
	ARoomManager* CheckNeighbours(int x, int y);
	void AddDoors(int x1, int y1, int x2, int y2);
};
