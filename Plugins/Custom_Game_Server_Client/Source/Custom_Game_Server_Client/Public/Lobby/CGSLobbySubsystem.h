#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Lobby/CGSLobbyTypes.h"
#include "CGSLobbySubsystem.generated.h"

UCLASS()
class CUSTOM_GAME_SERVER_CLIENT_API UCGSLobbySubsystem
	: public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Lobby APIs
	void CreateLobby(const FString& LobbyName, int32 MaxPlayers);
	void GetLobby();
	void GetLobbyByID(const FString& LobbyID);
	void GetLobbies();
	void JoinLobby(const FString& LobbyID);
	void LeaveLobby(const FString& LobbyID);
	void DestroyLobby(const FString& LobbyID);

	// Lobby State
	const FString& GetCurrentLobbyID() const;
	bool HasCurrentLobby() const;
	void KickPlayer(const FString& LobbyID,const FString& TargetUsername);

	const TArray<FCGSLobbyInfo>& GetCachedLobbyList() const;
	const FCGSLobbyDetailedInfo& GetCurrentLobbyInfo() const;

	//lobby delegates properties
	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyCreated OnLobbyCreated;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyCreateFailed OnLobbyCreateFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyFetched OnLobbyFetched;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyFetchFailed OnLobbyFetchFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyJoined OnLobbyJoined;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyJoinFailed OnLobbyJoinFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyLeft OnLobbyLeft;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyLeaveFailed OnLobbyLeaveFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyDestroyed OnLobbyDestroyed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyDestroyFailed OnLobbyDestroyFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyListReceived OnLobbyListReceived;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnLobbyListFailed OnLobbyListFailed;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnPlayerKicked OnPlayerKicked;

	UPROPERTY(BlueprintAssignable, Category="CGS|Lobby")
	FOnPlayerKickFailed OnPlayerKickFailed;

private:

	FString CurrentLobbyID;
	TArray<FCGSLobbyInfo> CachedLobbyList;
	FCGSLobbyDetailedInfo CurrentLobbyInfo;

	class UCGSSessionSubsystem* GetSessionSubsystem() const;
	bool GetSessionToken(FString& OutToken) const;

	void ClearCurrentLobbyState();
	void ClearCurrentLobbyStateIfMatches(const FString& LobbyID);
};