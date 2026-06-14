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