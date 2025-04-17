#include "Network_Structure/BrickGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
#include "AudioDevice.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

void UBrickGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Error, TEXT("BrickGAmeInstance::Init() called"));

}

void UBrickGameInstance::CreateGameSession()
{
    UE_LOG(LogTemp, Log, TEXT("Create Game Session"));
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            if (SessionInterface->GetNamedSession(FName("GameSession")) != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Need to delete and recreate"));
                SessionInterface->DestroySession(FName("GameSession"));
                SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBrickGameInstance::OnDestroySessionComplete);
                return; // 삭제 완료 후 콜백에서 다시 생성
            }

            CreateSessionInternal();
        }
    }
}

void UBrickGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Success Delete Session. ReCreate Session"));
        CreateSessionInternal();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed Delete Session"));
    }
}

void UBrickGameInstance::CreateSessionInternal()
{
    UE_LOG(LogTemp, Log, TEXT("Create Session"));

    SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = true;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->bIsDedicated = false;

    SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBrickGameInstance::OnCreateSessionComplete);

    ULocalPlayer* LocalPlayer = GetFirstGamePlayer();
    if (!LocalPlayer || !LocalPlayer->GetPreferredUniqueNetId().IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("No Valid UniqueNetId"));
        return;
    }

    TSharedPtr<const FUniqueNetId> UserId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
    SessionInterface->CreateSession(*UserId, FName("GameSession"), *SessionSettings);
}

void UBrickGameInstance::JoinGameSession(const FString& IPAddress)
{
    UE_LOG(LogTemp, Log, TEXT("Join Game Session"));
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        //PC->ClientTravel(IPAddress, TRAVEL_Absolute);
        PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
    }
}

void UBrickGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Success Create Session. Go to map..."));
        UGameplayStatics::OpenLevel(this, FName("/Game/Maps/LobbyLevel"), true, "listen");
        //GetWorld()->ServerTravel(TEXT("/Game/Maps/LobbyLevel?listen")); // 실제 맵명으로 변경
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed Create Session"));
    }
}