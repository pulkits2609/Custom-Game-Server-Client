#include "UI/Lobby/CGSLobbyEntryWidget.h"

#include "Lobby/CGSLobbySubsystem.h"

void UCGSLobbyEntryWidget::SetLobbyInfo(
	const FCGSLobbyInfo& InLobbyInfo)
{
	LobbyInfo = InLobbyInfo;
}

void UCGSLobbyEntryWidget::JoinCurrentLobby()
{
	if (UCGSLobbySubsystem* Lobby = GetLobbySubsystem())
	{
		Lobby->JoinLobby(
			LobbyInfo.LobbyID);
	}
}

const FCGSLobbyInfo&
UCGSLobbyEntryWidget::GetLobbyInfo() const
{
	return LobbyInfo;
}

void UCGSLobbyEntryWidget::SetTargetUsername(
    const FString& InUsername)
{
    TargetUsername = InUsername;
}

void UCGSLobbyEntryWidget::KickPlayer()
{
    UCGSLobbySubsystem* Lobby =
        GetLobbySubsystem();

    if (!Lobby)
    {
        return;
    }

    Lobby->KickPlayer(
        Lobby->GetCurrentLobbyID(),
        TargetUsername);
}