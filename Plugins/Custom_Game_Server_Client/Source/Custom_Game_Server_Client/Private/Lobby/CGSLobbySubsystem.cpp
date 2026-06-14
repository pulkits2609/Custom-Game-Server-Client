#include "Lobby/CGSLobbySubsystem.h"

#include "Http/CGSHttpHelpers.h"
#include "Lobby/Requests/CGSLobbyRequests.h"
#include "Sessions/CGSSessionSubsystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void UCGSLobbySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("CGS Lobby Subsystem Initialized"));
}

void UCGSLobbySubsystem::Deinitialize()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("CGS Lobby Subsystem Deinitialized"));

	Super::Deinitialize();
}

UCGSSessionSubsystem* UCGSLobbySubsystem::GetSessionSubsystem() const
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetSubsystem<UCGSSessionSubsystem>();
	}

	return nullptr;
}

bool UCGSLobbySubsystem::GetSessionToken(FString& OutToken) const
{
	OutToken.Empty();

	if (UCGSSessionSubsystem* SessionSubsystem = GetSessionSubsystem())
	{
		if (SessionSubsystem->IsLoggedIn())
		{
			OutToken = SessionSubsystem->GetSessionToken();
			return true;
		}
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Cannot perform lobby action - not logged in"));

	return false;
}

void UCGSLobbySubsystem::ClearCurrentLobbyState()
{
	CurrentLobbyID.Empty();
	CurrentLobbyInfo = FCGSLobbyDetailedInfo{};
	CachedLobbyList.Reset();
}

void UCGSLobbySubsystem::ClearCurrentLobbyStateIfMatches(const FString& LobbyID)
{
	if (CurrentLobbyID == LobbyID)
	{
		ClearCurrentLobbyState();
	}
}

void UCGSLobbySubsystem::CreateLobby(const FString& LobbyName, int32 MaxPlayers)
{
	if (LobbyName.IsEmpty() || MaxPlayers <= 0)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("CreateLobby failed - invalid lobby name or max players"));
		
		OnLobbyCreateFailed.Broadcast(TEXT("CreateLobby failed - invalid lobby name or max players"));
		
		return;
	}

	FString SessionToken;
	if (!GetSessionToken(SessionToken))
	{
		return;
	}

	const FString RequestBody =
		CGSLobbyRequests::BuildCreateLobbyBody(LobbyName, MaxPlayers);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateJsonRequest(
			CGSLobbyRequests::BuildCreateLobbyPath(),
			TEXT("POST"),
			RequestBody,
			&SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("CreateLobby request failed"));

				OnLobbyCreateFailed.Broadcast(TEXT("CreateLobby request failed"));
				
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("CreateLobby failed: %s"),
					*Response->GetContentAsString());

				OnLobbyCreateFailed.Broadcast(Response->GetContentAsString());
				
				return;
			}

			FCGSLobbyActionResponse ParsedResponse;
			if (!CGSLobbyRequests::TryParseLobbyActionResponse(
				Response->GetContentAsString(),
				ParsedResponse))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse CreateLobby response"));
				
				OnLobbyCreateFailed.Broadcast(TEXT("Failed to parse CreateLobby response"));
				
				return;
			}

			CurrentLobbyID = ParsedResponse.LobbyID;
			
			OnLobbyCreated.Broadcast(ParsedResponse);

			UE_LOG(LogTemp, Warning, TEXT("Lobby Created"));
			UE_LOG(LogTemp, Warning, TEXT("LobbyID: %s"), *CurrentLobbyID);

			if (!CurrentLobbyID.IsEmpty())
			{
				GetLobbyByID(CurrentLobbyID);
			}
		});

	Request->ProcessRequest();
}

void UCGSLobbySubsystem::GetLobby()
{
	if (CurrentLobbyID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot GetLobby - no current lobby ID"));
		return;
	}

	GetLobbyByID(CurrentLobbyID);
}

void UCGSLobbySubsystem::GetLobbyByID(const FString& LobbyID)
{
	if (LobbyID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("GetLobbyByID failed - empty LobbyID"));
		OnLobbyFetchFailed.Broadcast(TEXT("Get LobbyByID Failed - Empty lobby ID"));
		return;
	}

	FString SessionToken;
	if (!GetSessionToken(SessionToken))
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			CGSLobbyRequests::BuildLobbyPath(LobbyID),
			TEXT("GET"));

	CGSHttp::SetAuthHeader(*Request, SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this, LobbyID](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("GetLobby failed"));
				OnLobbyFetchFailed.Broadcast(TEXT("Get Lobby Failed"));
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("GetLobby failed: %s"),
					*Response->GetContentAsString());
				
				OnLobbyFetchFailed.Broadcast(*Response->GetContentAsString());	

				return;
			}

			TSharedPtr<FJsonObject> JsonObject;
			if (!CGSHttp::TryDeserializeJsonObject(Response->GetContentAsString(), JsonObject))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse GetLobby response"));
				
				OnLobbyFetchFailed.Broadcast(TEXT("Failed to parse GetLobby response"));
				
				return;
			}

			FCGSLobbyDetailedInfo ParsedLobby;
			if (!CGSLobbyRequests::TryParseLobbyDetailedInfo(JsonObject, ParsedLobby))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to extract lobby fields"));
				
				OnLobbyFetchFailed.Broadcast(TEXT("Failed to extract lobby fields"));
				
				return;
			}

			CurrentLobbyID = LobbyID;
			CurrentLobbyInfo = ParsedLobby;

			OnLobbyFetched.Broadcast(CurrentLobbyInfo);

			UE_LOG(LogTemp, Warning, TEXT("Lobby Fetched"));
			UE_LOG(LogTemp, Warning, TEXT("LobbyID: %s"), *CurrentLobbyInfo.LobbyID);
			UE_LOG(LogTemp, Warning, TEXT("LobbyName: %s"), *CurrentLobbyInfo.LobbyName);
			UE_LOG(LogTemp, Warning, TEXT("Players: %d/%d"), CurrentLobbyInfo.CurrentPlayers, CurrentLobbyInfo.MaxPlayers);

			if (!CurrentLobbyInfo.HostID.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("HostID: %s"), *CurrentLobbyInfo.HostID);
			}
		});

	Request->ProcessRequest();
}

void UCGSLobbySubsystem::GetLobbies()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			CGSLobbyRequests::BuildListLobbiesPath(),
			TEXT("GET"));

	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("GetLobbies request failed"));
				OnLobbyListFailed.Broadcast(TEXT("GetLobbies request failed"));
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("GetLobbies failed: %s"),
					*Response->GetContentAsString());
				OnLobbyListFailed.Broadcast(Response->GetContentAsString());
				return;
			}

			TArray<FCGSLobbyInfo> ParsedLobbies;
			if (!CGSLobbyRequests::TryParseLobbyList(
				Response->GetContentAsString(),
				ParsedLobbies))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse lobby list"));
				OnLobbyListFailed.Broadcast(TEXT("Failed to parse lobby list"));
				return;
			}

			CachedLobbyList = MoveTemp(ParsedLobbies);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Lobby list received: %d lobby(s)"),
				CachedLobbyList.Num());
			
			OnLobbyListReceived.Broadcast(CachedLobbyList);

			for (const FCGSLobbyInfo& Lobby : CachedLobbyList)
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("Lobby: %s | %s | %d/%d | Full: %s"),
					*Lobby.LobbyID,
					*Lobby.LobbyName,
					Lobby.CurrentPlayers,
					Lobby.MaxPlayers,
					Lobby.bIsFull ? TEXT("true") : TEXT("false"));
			}
		});

	Request->ProcessRequest();
}

void UCGSLobbySubsystem::JoinLobby(const FString& LobbyID)
{
	if (LobbyID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("JoinLobby failed - empty LobbyID"));
		OnLobbyJoinFailed.Broadcast(TEXT("LobbyID is empty"));
		return;
	}

	FString SessionToken;
	if (!GetSessionToken(SessionToken))
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			CGSLobbyRequests::BuildJoinLobbyPath(LobbyID),
			TEXT("POST"));

	CGSHttp::SetAuthHeader(*Request, SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this, LobbyID](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("JoinLobby request failed"));
				OnLobbyJoinFailed.Broadcast(TEXT("JoinLobby request failed"));
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("JoinLobby failed: %s"),
					*Response->GetContentAsString());
				OnLobbyJoinFailed.Broadcast(Response->GetContentAsString());
				return;
			}

			FCGSLobbyActionResponse ParsedResponse;
			if (!CGSLobbyRequests::TryParseLobbyActionResponse(
				Response->GetContentAsString(),
				ParsedResponse))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to parse JoinLobby response"));
				OnLobbyJoinFailed.Broadcast(TEXT("Failed to parse JoinLobby response"));
				return;
			}

			CurrentLobbyID = LobbyID;
			OnLobbyJoined.Broadcast(ParsedResponse);

			UE_LOG(LogTemp, Warning, TEXT("Joined Lobby"));
			UE_LOG(LogTemp, Warning, TEXT("LobbyID: %s"), *CurrentLobbyID);

			GetLobbyByID(CurrentLobbyID);
		});

	Request->ProcessRequest();
}

void UCGSLobbySubsystem::LeaveLobby(const FString& LobbyID)
{
	if (LobbyID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("LeaveLobby failed - empty LobbyID"));
		OnLobbyLeaveFailed.Broadcast(TEXT("LobbyID is empty"));
		return;
	}

	FString SessionToken;
	if (!GetSessionToken(SessionToken))
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			CGSLobbyRequests::BuildLeaveLobbyPath(LobbyID),
			TEXT("POST"));

	CGSHttp::SetAuthHeader(*Request, SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this, LobbyID](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("LeaveLobby request failed"));
				OnLobbyLeaveFailed.Broadcast(TEXT("LeaveLobby request failed"));
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("LeaveLobby failed: %s"),
					*Response->GetContentAsString());
				OnLobbyLeaveFailed.Broadcast(Response->GetContentAsString());
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("LeaveLobby success: %s"), *Response->GetContentAsString());

			ClearCurrentLobbyStateIfMatches(LobbyID);
			OnLobbyLeft.Broadcast(LobbyID);
		});

	Request->ProcessRequest();
}

void UCGSLobbySubsystem::DestroyLobby(const FString& LobbyID)
{
	if (LobbyID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("DestroyLobby failed - empty LobbyID"));
		OnLobbyDestroyFailed.Broadcast(TEXT("LobbyID is empty"));
		return;
	}

	FString SessionToken;
	if (!GetSessionToken(SessionToken))
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			CGSLobbyRequests::BuildDestroyLobbyPath(LobbyID),
			TEXT("POST"));

	CGSHttp::SetAuthHeader(*Request, SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this, LobbyID](FHttpRequestPtr RequestPtr, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("DestroyLobby request failed"));
				OnLobbyDestroyFailed.Broadcast(TEXT("DestroyLobby request failed"));
				return;
			}

			if (Response->GetResponseCode() != 200)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("DestroyLobby failed: %s"),
					*Response->GetContentAsString());
				OnLobbyDestroyFailed.Broadcast(Response->GetContentAsString());	
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("DestroyLobby success: %s"), *Response->GetContentAsString());

			ClearCurrentLobbyStateIfMatches(LobbyID);
			OnLobbyDestroyed.Broadcast(LobbyID);
		});

	Request->ProcessRequest();
}

const FString& UCGSLobbySubsystem::GetCurrentLobbyID() const
{
	return CurrentLobbyID;
}

bool UCGSLobbySubsystem::HasCurrentLobby() const
{
	return !CurrentLobbyID.IsEmpty();
}

const TArray<FCGSLobbyInfo>& UCGSLobbySubsystem::GetCachedLobbyList() const
{
	return CachedLobbyList;
}

const FCGSLobbyDetailedInfo& UCGSLobbySubsystem::GetCurrentLobbyInfo() const
{
	return CurrentLobbyInfo;
}