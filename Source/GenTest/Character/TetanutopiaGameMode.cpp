#include "TetanutopiaGameMode.h"
#include "PlayerRobot.h"

ATetanutopiaGameMode::ATetanutopiaGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = APlayerRobot::StaticClass();
}