#pragma once

#include "CoreMinimal.h"
#include "CGSLobbyTypes.generated.h"

USTRUCT(BlueprintType)
struct FCGSLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString LobbyID;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString LobbyName;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	bool bIsFull = false;
};

USTRUCT(BlueprintType)
struct FCGSLobbyDetailedInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString LobbyID;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString LobbyName;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	bool bIsFull = false;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString HostID;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	TArray<FString> Members;
};

USTRUCT(BlueprintType)
struct FCGSLobbyActionResponse
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString Message;

	UPROPERTY(BlueprintReadOnly, Category="CGS|Lobby")
	FString LobbyID;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyCreated,
    const FCGSLobbyActionResponse&,
    Response);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyCreateFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyFetched,
    const FCGSLobbyDetailedInfo&,
    LobbyInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyFetchFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyJoined,
    const FCGSLobbyActionResponse&,
    Response);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyJoinFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyLeft,
    const FString&,
    LobbyID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyLeaveFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyDestroyed,
    const FString&,
    LobbyID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyDestroyFailed,
    const FString&,
    Error);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyListReceived,
    TArray<FCGSLobbyInfo>,
    Lobbies);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnLobbyListFailed,
    const FString&,
    Error);