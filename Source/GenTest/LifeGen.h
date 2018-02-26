// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LifeGen.generated.h"

UCLASS(Blueprintable)
class GENTEST_API ALifeGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALifeGen();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int livingChance = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int deathLimit = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int birthLimit = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int steps = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform t;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//const int m_width = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//static const int height = 50;*/
	bool map[40][100];

	const int m_width = 40;
	const int m_height = 100;

	UFUNCTION(BlueprintCallable, Category = "GameDev|LifeGen")
		void CreateMap();
	UFUNCTION(BlueprintCallable, Category = "GameDev|LifeGen")
		void DrawMap();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameDev|LifeGen")
	void SpawnISM(FVector trans);

	UFUNCTION(BlueprintCallable, Category = "GameDev|LifeGen")
		void DoSimulationStep();

	UFUNCTION(BlueprintCallable, Category = "GameDev|LifeGen")
		void DeleteRandomBlock();

	int CountAliveNeighbours(int x, int y);

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	static const int x = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	static const int y = 1;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};