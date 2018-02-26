// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "LevelGen.generated.h"

/**
 * 
 */
UCLASS()
class GENTEST_API ALevelGen : public AWorldSettings
{
	GENERATED_BODY()
	
	
	
public:
	int* Gen();
	UPROPERTY(EditAnywhere)
	int x = 3;
	UPROPERTY(EditAnywhere)
	int y = 3;
	enum rooms { START, END, BOSS, CHEST, EMPTY };
	enum connections { RD, LD, RU, LU, RL, UD, C };

};

