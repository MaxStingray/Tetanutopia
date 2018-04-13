#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Translation.generated.h"

UCLASS()
class GENTEST_API UTranslation : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Localization"), Category = "Locale")
	static void ChangeLocalization(FString target);
};
