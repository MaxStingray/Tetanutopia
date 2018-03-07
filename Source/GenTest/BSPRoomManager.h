

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "BSP.h"
#include "BSPRoomManager.generated.h"


UENUM(BlueprintType)
enum class Avb : uint8
{
	EMPTY		UMETA(DisplayName = "Empty tile"),
	WALL		UMETA(DisplayName = "Wall tile"),
	DOOR			UMETA(DisplayName = "Door tile"),
	PROP		UMETA(DisplayName = "Prop tile")
};

UCLASS()
class GENTEST_API ABSPRoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABSPRoomManager();

	void init();

	FVector location;

	TArray<int32> Tiles;

	int width;
	int height;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Floors;
	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Walls;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DrawRoom();
	void PlaceProps();
	void SetAvb(int row, int col, int value);
	int GetAvb(int row, int col);
	void PopulateRoom();
	bool TestPropPlacement(int x, int y, int sizeX, int sizeY);
	void SetPropPlacement(int x, int y, int sizeX, int sizeY);
};
