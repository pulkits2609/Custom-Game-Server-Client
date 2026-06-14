#pragma once

#include "CoreMinimal.h"
#include "Lobby/CGSLobbyTypes.h"

class FJsonObject;

namespace CGSLobbyRequests
{
	FString BuildCreateLobbyBody(const FString& LobbyName, int32 MaxPlayers);

	FString BuildCreateLobbyPath();
	FString BuildListLobbiesPath();
	FString BuildLobbyPath(const FString& LobbyID);
	FString BuildJoinLobbyPath(const FString& LobbyID);
	FString BuildLeaveLobbyPath(const FString& LobbyID);
	FString BuildDestroyLobbyPath(const FString& LobbyID);

	bool TryParseLobbyInfo(const TSharedPtr<FJsonObject>& JsonObject, FCGSLobbyInfo& OutLobby);
	bool TryParseLobbyDetailedInfo(const TSharedPtr<FJsonObject>& JsonObject, FCGSLobbyDetailedInfo& OutLobby);
	bool TryParseLobbyActionResponse(const FString& JsonString, FCGSLobbyActionResponse& OutResponse);
	bool TryParseLobbyList(const FString& JsonString, TArray<FCGSLobbyInfo>& OutLobbies);
}