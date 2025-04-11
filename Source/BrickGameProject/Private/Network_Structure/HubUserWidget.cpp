// Fill out your copyright notice in the Description page of Project Settings.


#include "Network_Structure/HubUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Network_Structure/BrickGameInstance.h"

void UHubUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreateSessionButton)
        CreateSessionButton->OnClicked.AddDynamic(this, &UHubUserWidget::OnCreateSessionClicked);

    if (JoinSessionButton)
        JoinSessionButton->OnClicked.AddDynamic(this, &UHubUserWidget::OnJoinSessionClicked);
}

void UHubUserWidget::OnCreateSessionClicked()
{
    UE_LOG(LogTemp, Log, TEXT("CreateSession Button Clicked"));
    if (UBrickGameInstance* GI = Cast<UBrickGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        GI->CreateGameSession();  // 세션 생성 및 맵 이동
    }
}

void UHubUserWidget::OnJoinSessionClicked()
{
    UE_LOG(LogTemp, Log, TEXT("JoinSession Button Clicked"));
    if (!IPAddressBox) return;

    FString IPAddress = IPAddressBox->GetText().ToString();

    if (UBrickGameInstance* GI = Cast<UBrickGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        //?game=/Game/Maps/LobbyLevel
        GI->JoinGameSession(IPAddress);  // IP로 클라이언트 접속
    }
}