#include "Translation.h"

void UTranslation::ChangeLocalization(FString target)
{
	FInternationalization::Get().SetCurrentCulture(target);
}

