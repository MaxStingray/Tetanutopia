

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "BSP.h"
#include "BSPRoomManager.generated.h"

UCLASS()
class GENTEST_API ABSPRoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABSPRoomManager();

	FVector location;

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
	
};
