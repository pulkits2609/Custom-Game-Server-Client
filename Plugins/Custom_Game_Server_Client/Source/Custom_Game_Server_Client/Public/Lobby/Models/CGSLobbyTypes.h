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