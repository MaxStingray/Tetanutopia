// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Tile.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "RoomManager.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
public:
	int index;
	FTransform trans;
	UHierarchicalInstancedStaticMeshComponent * parentComp;
};

//enumerator of room types to be assigned by parameters at runtime
UENUM(BlueprintType)
enum class RoomType : uint8
{
	RT_TURRET		UMETA(DisplayName = "turret room"),
	RT_START		UMETA(DisplayName = "start room"),
	RT_END			UMETA(DisplayName = "end room"),
	RT_ENEMY		UMETA(DisplayName = "enemy room"),
	RT_WEAPON		UMETA(DisplayName = "weapon room"),
	RT_GENERIC		UMETA(DisplayName = "generic room (placeholder roomtype)")//remove this eventually
};

UCLASS(Blueprintable)
class GENTEST_API ARoomManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
		RoomType assignedRoomType;

	UPROPERTY(EditAnywhere)
		bool hasTurrets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMyActor> ToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Turret;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Floors;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Walls;

	bool visited = false;
	int x;
	int y;
	float time = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* Floor1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* Wall;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;
	//collection of floor tile locations for this room
	UPROPERTY(EditAnywhere)
		TMap<FString, FVector> floorMap;

	UPROPERTY(EditAnywhere)
		TArray<FVector> floorCoords;
	//collection of wall tile locations for this room
	UPROPERTY(EditAnywhere)
		TArray<FVector> wallCoords;

	UFUNCTION(BlueprintCallable)
		void Spawn(FVector location, TSubclassOf<AActor> actor);

	UFUNCTION(BlueprintCallable)
		void DrawWall(int startX, int startY, int endX, int endY);

	UFUNCTION(BlueprintCallable)
		void GenerateRoom(int sizeX, int sizeY);

	UFUNCTION(BlueprintCallable)
		void DrawInteriorRoom(int startX, int startY, int width, int height);

	UFUNCTION(BlueprintCallable)
		void PlaceAtCoOrdinate(int x, int y, TSubclassOf<AActor> actor);

	UFUNCTION(BlueprintCallable)
		void TurretRing();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameDev|RoomManager")
		void SpawnISM(FTransform trans, UHierarchicalInstancedStaticMeshComponent* targetMesh);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameDev|RoomManager")
		void DeleteISM(int32 index);

protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintCallable)
		TMap<FString, FVector> GetNodeNeighbors(FVector start);

	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};


