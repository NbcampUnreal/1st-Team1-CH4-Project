#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGameTeam : uint8
{
	None  UMETA(DisplayName = "None"),
	Red  UMETA(DisplayName = "Red"),
	Blue  UMETA(DisplayName = "Blue"),
	Green  UMETA(DisplayName = "Green"),
	Yellow  UMETA(DisplayName = "Yellow")
};