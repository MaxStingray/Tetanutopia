

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "BSPRoomManager.h"
#include "BSP.generated.h"

USTRUCT(BlueprintType)
struct FChunk
{
	GENERATED_BODY()
public:
	int parent;
	int TopChild;
	int BottomChild;
	int RightChild;
	int LeftChild;
	FVector chunkCenter;
	int chunkWidth;
	int chunkHeight;
	int splitX;
	int splitY;
	bool horizontal;
};

UCLASS()
class GENTEST_API ABSP : public AActor
{
	GENERATED_BODY()

public:
	ABSP();
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int levelWidth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int levelHeight = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int minWidth = 16;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int minHeight = 16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int subDivisions = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int unitSize = 100;

	TArray<FChunk> chunks;
	TArray<ABSPRoomManager*> rms;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Floors;
	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Walls;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Floor;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Wall;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Door;


	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Barrels;

	UFUNCTION(BlueprintCallable, Category = "GameDev|BSP")
		ABSPRoomManager* SpawnRM(FVector location);
	UFUNCTION(BlueprintCallable)
		void Spawn(FVector location, TSubclassOf<AActor> actor);
	UFUNCTION(BlueprintCallable)
		void SpawnTransform(FTransform position, TSubclassOf<AActor> actor);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameDev|BSP")
		void SpawnISM(FTransform trans, UHierarchicalInstancedStaticMeshComponent* targetMesh);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABSPRoomManager> rm;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GameDev|BSP")
		void CreateLevel();

	bool Split(int i);
	
	UFUNCTION(BlueprintCallable, Category = "GameDev|BSP")
		void DrawMap();

	void AddRoomManagers();
	void DrawRooms();
	void AddDoors();
};