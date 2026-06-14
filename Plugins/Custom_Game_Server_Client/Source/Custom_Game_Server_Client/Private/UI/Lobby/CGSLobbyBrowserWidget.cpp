#include "UI/Lobby/CGSLobbyBrowserWidget.h"

#include "Lobby/CGSLobbySubsystem.h"

void UCGSLobbyBrowserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UCGSLobbySubsystem* Lobby = GetLobbySubsystem())
	{
		Lobby->OnLobbyListReceived.AddDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyListReceived);

		Lobby->OnLobbyListFailed.AddDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyListFailed);

		Lobby->OnLobbyJoined.AddDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyJoined);

		Lobby->OnLobbyJoinFailed.AddDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyJoinFailed);
	}
}

void UCGSLobbyBrowserWidget::NativeDestruct()
{
	if (UCGSLobbySubsystem* Lobby = GetLobbySubsystem())
	{
		Lobby->OnLobbyListReceived.RemoveDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyListReceived);

		Lobby->OnLobbyListFailed.RemoveDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyListFailed);

		Lobby->OnLobbyJoined.RemoveDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyJoined);

		Lobby->OnLobbyJoinFailed.RemoveDynamic(
			this,
			&UCGSLobbyBrowserWidget::HandleLobbyJoinFailed);
	}

	Super::NativeDestruct();
}

void UCGSLobbyBrowserWidget::RefreshLobbyList()
{
	if (UCGSLobbySubsystem* Lobby = GetLobbySubsystem())
	{
		Lobby->GetLobbies();
	}
}

void UCGSLobbyBrowserWidget::JoinLobby(
	const FString& LobbyID)
{
	if (UCGSLobbySubsystem* Lobby = GetLobbySubsystem())
	{
		Lobby->JoinLobby(LobbyID);
	}
}

void UCGSLobbyBrowserWidget::HandleLobbyListReceived(
	const TArray<FCGSLobbyInfo> Lobbies)
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Received %d Lobbies"),
		Lobbies.Num());

	for (const FCGSLobbyInfo& Lobby : Lobbies)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Lobby: %s (%d/%d)"),
			*Lobby.LobbyName,
			Lobby.CurrentPlayers,
			Lobby.MaxPlayers);
	}
}

void UCGSLobbyBrowserWidget::HandleLobbyListFailed(
	const FString& Error)
{
	UE_LOG(
		LogTemp,
		Error,
		TEXT("Lobby List Failed: %s"),
		*Error);
}

void UCGSLobbyBrowserWidget::HandleLobbyJoined(
	const FCGSLobbyActionResponse& Response)
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Joined Lobby: %s"),
		*Response.LobbyID);
}

void UCGSLobbyBrowserWidget::HandleLobbyJoinFailed(
	const FString& Error)
{
	UE_LOG(
		LogTemp,
		Error,
		TEXT("Join Lobby Failed: %s"),
		*Error);
}