#include "BrickGameMode.h"
#include "BricCharacter.h"
#include "BricPlayerController.h"

ABrickGameMode::ABrickGameMode()
	: MaxPlayerCount(4)
	, GameDuration(300.0f)
	, bGameInProgress(false)
{
	DefaultPawnClass = ABricCharacter::StaticClass(); //클래스 이름을 통해 호출해주는 것(객체를 생성하지 않고도 객체를 선언해주는 것)
	PlayerControllerClass = ABricPlayerController::StaticClass();

}

void ABrickGameMode::StartGame()
{
}

void ABrickGameMode::EndGame()
{
}

void ABrickGameMode::CheckWinCondition()
{
}
