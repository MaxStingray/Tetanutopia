

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "OutDoorLevelGen.generated.h"
UCLASS()
class GENTEST_API AOutDoorLevelGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOutDoorLevelGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	const int width = 40;
	const int height = 98;
	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Wall;

	UPROPERTY(EditAnywhere)
		UHierarchicalInstancedStaticMeshComponent *Noise;

	UPROPERTY(EditAnywhere)
		UStaticMesh* rock;

	UPROPERTY(EditAnywhere)
		UStaticMesh* spike;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameDev|OutDoor")
		void SpawnISM(FTransform trans, UHierarchicalInstancedStaticMeshComponent* targetMesh);
	
	UFUNCTION(BlueprintCallable, Category = "GameDev|OutDoor")
		void AddNoise();
	
	UFUNCTION(BlueprintCallable, Category = "GameDev|OutDoor")
		void CreateWall();
};
