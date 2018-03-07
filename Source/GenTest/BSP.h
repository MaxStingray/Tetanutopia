

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "BSP.generated.h"

USTRUCT(BlueprintType)
struct FChunk
{
	GENERATED_BODY()
public:
	FChunk* parent;
	FChunk* TopChild;
	FChunk* BottomChild;
	FChunk* RightChild;
	FChunk* LeftChild;
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
		int levelWidth = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int levelHeight = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int minWidth = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int minHeight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int subDivisions = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int unitSize = 100;

	TArray<FChunk> chunks;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Floors;
	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Walls;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Floor;
	UPROPERTY(EditAnywhere)
		UStaticMesh* Wall;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameDev|BSP")
		void SpawnISM(FTransform trans, UHierarchicalInstancedStaticMeshComponent* targetMesh);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "GameDev|BSP")
		void CreateLevel();

	bool Split(int i);
	
	UFUNCTION(BlueprintCallable, Category = "GameDev|BSP")
		void DrawMap();



};