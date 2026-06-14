#include "UI/Lobby/CGSCreateLobbyWidget.h"

#include "Lobby/CGSLobbySubsystem.h"

void UCGSCreateLobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UCGSLobbySubsystem* Lobby =
        GetLobbySubsystem())
    {
        Lobby->OnLobbyCreated.AddDynamic(
            this,
            &UCGSCreateLobbyWidget::HandleLobbyCreated);

        Lobby->OnLobbyCreateFailed.AddDynamic(
            this,
            &UCGSCreateLobbyWidget::HandleLobbyCreateFailed);
    }
}

void UCGSCreateLobbyWidget::NativeDestruct()
{
    if (UCGSLobbySubsystem* Lobby =
        GetLobbySubsystem())
    {
        Lobby->OnLobbyCreated.RemoveDynamic(
            this,
            &UCGSCreateLobbyWidget::HandleLobbyCreated);

        Lobby->OnLobbyCreateFailed.RemoveDynamic(
            this,
            &UCGSCreateLobbyWidget::HandleLobbyCreateFailed);
    }

    Super::NativeDestruct();
}

void UCGSCreateLobbyWidget::CreateLobby(
    const FString& LobbyName,
    int32 MaxPlayers)
{
    if (UCGSLobbySubsystem* Lobby =
        GetLobbySubsystem())
    {
        Lobby->CreateLobby(
            LobbyName,
            MaxPlayers);
    }
}

void UCGSCreateLobbyWidget::HandleLobbyCreated(
    const FCGSLobbyActionResponse& Response)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Lobby Created: %s"),
        *Response.LobbyID);
}

void UCGSCreateLobbyWidget::HandleLobbyCreateFailed(
    const FString& Error)
{
    UE_LOG(
        LogTemp,
        Error,
        TEXT("Create Lobby Failed: %s"),
        *Error);
}