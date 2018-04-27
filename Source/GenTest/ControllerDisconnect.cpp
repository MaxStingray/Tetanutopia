#include "ControllerDisconnect.h"
#include "SlateBasics.h"

bool UControllerDisconnect::IsGamePadConnected()
{
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	{
		return true;
	}
	return false;
}

