// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/LegoGameMode.h"
#include "Character/LegoPlayerController.h"
#include "Character/LegoCharacter.h"

ALegoGameMode::ALegoGameMode()
{
	DefaultPawnClass = ALegoCharacter::StaticClass(); //클래스 이름을 통해 호출해주는 것(객체를 생성하지 않고도 객체를 선언해주는 것)
	PlayerControllerClass = ALegoPlayerController::StaticClass();
}