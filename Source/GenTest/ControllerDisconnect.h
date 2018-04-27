#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ControllerDisconnect.generated.h"

UCLASS()
class GENTEST_API UControllerDisconnect : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GamepadConnected"))
	static bool IsGamePadConnected();
};
