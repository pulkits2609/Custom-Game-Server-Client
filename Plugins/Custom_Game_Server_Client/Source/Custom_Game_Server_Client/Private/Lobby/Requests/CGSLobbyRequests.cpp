#include "Lobby/Requests/CGSLobbyRequests.h"

#include "Http/CGSHttpHelpers.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

namespace CGSLobbyRequests
{
	static bool ParseBasicLobbyInfo(
		const TSharedPtr<FJsonObject>& JsonObject,
		FCGSLobbyInfo& OutLobby)
	{
		if (!JsonObject.IsValid())
		{
			return false;
		}

		JsonObject->TryGetStringField(TEXT("LobbyID"), OutLobby.LobbyID);
		JsonObject->TryGetStringField(TEXT("LobbyName"), OutLobby.LobbyName);

		double CurrentPlayersValue = 0.0;
		double MaxPlayersValue = 0.0;
		bool bIsFullValue = false;

		JsonObject->TryGetNumberField(TEXT("CurrentPlayers"), CurrentPlayersValue);
		JsonObject->TryGetNumberField(TEXT("MaxPlayers"), MaxPlayersValue);
		JsonObject->TryGetBoolField(TEXT("IsFull"), bIsFullValue);

		OutLobby.CurrentPlayers = FMath::RoundToInt(CurrentPlayersValue);
		OutLobby.MaxPlayers = FMath::RoundToInt(MaxPlayersValue);
		OutLobby.bIsFull = bIsFullValue;

		return true;
	}

	FString BuildCreateLobbyBody(const FString& LobbyName, int32 MaxPlayers)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetStringField(TEXT("lobbyName"), LobbyName);
		JsonObject->SetNumberField(TEXT("maxPlayers"), MaxPlayers);

		return CGSHttp::SerializeJsonObject(JsonObject);
	}

	FString BuildCreateLobbyPath()
	{
		return TEXT("/lobby/create");
	}

	FString BuildListLobbiesPath()
	{
		return TEXT("/lobby/list");
	}

	FString BuildLobbyPath(const FString& LobbyID)
	{
		return FString::Printf(TEXT("/lobby/fetch/%s"), *LobbyID);
	}

	FString BuildJoinLobbyPath(const FString& LobbyID)
	{
		return FString::Printf(TEXT("/lobby/join/%s"), *LobbyID);
	}

	FString BuildLeaveLobbyPath(const FString& LobbyID)
	{
		return FString::Printf(TEXT("/lobby/leave/%s"), *LobbyID);
	}

	FString BuildDestroyLobbyPath(const FString& LobbyID)
	{
		return FString::Printf(TEXT("/lobby/destroy/%s"), *LobbyID);
	}

	FString BuildKickLobbyPath(
		const FString& LobbyID,
		const FString& TargetUsername)
	{
		return FString::Printf(
			TEXT("/lobby/kick/%s/%s"),
			*LobbyID,
			*TargetUsername);
	}

	bool TryParseLobbyInfo(const TSharedPtr<FJsonObject>& JsonObject, FCGSLobbyInfo& OutLobby)
	{
		OutLobby = FCGSLobbyInfo{};
		return ParseBasicLobbyInfo(JsonObject, OutLobby);
	}

	bool TryParseLobbyDetailedInfo(const TSharedPtr<FJsonObject>& JsonObject, FCGSLobbyDetailedInfo& OutLobby)
	{
		OutLobby = FCGSLobbyDetailedInfo{};

		if (!JsonObject.IsValid())
		{
			return false;
		}

		JsonObject->TryGetStringField(TEXT("LobbyID"), OutLobby.LobbyID);
		JsonObject->TryGetStringField(TEXT("LobbyName"), OutLobby.LobbyName);

		double CurrentPlayersValue = 0.0;
		double MaxPlayersValue = 0.0;
		bool bIsFullValue = false;

		JsonObject->TryGetNumberField(TEXT("CurrentPlayers"), CurrentPlayersValue);
		JsonObject->TryGetNumberField(TEXT("MaxPlayers"), MaxPlayersValue);
		JsonObject->TryGetBoolField(TEXT("IsFull"), bIsFullValue);

		OutLobby.CurrentPlayers = FMath::RoundToInt(CurrentPlayersValue);
		OutLobby.MaxPlayers = FMath::RoundToInt(MaxPlayersValue);
		OutLobby.bIsFull = bIsFullValue;

		JsonObject->TryGetStringField(TEXT("HostID"), OutLobby.HostID);

		const TArray<TSharedPtr<FJsonValue>>* MembersArray = nullptr;
		if (JsonObject->TryGetArrayField(TEXT("Members"), MembersArray) && MembersArray)
		{
			for (const TSharedPtr<FJsonValue>& Value : *MembersArray)
			{
				if (Value.IsValid())
				{
					OutLobby.Members.Add(Value->AsString());
				}
			}
		}

		return true;
	}

	bool TryParseLobbyActionResponse(const FString& JsonString, FCGSLobbyActionResponse& OutResponse)
	{
		OutResponse = FCGSLobbyActionResponse{};

		TSharedPtr<FJsonObject> JsonObject;
		if (!CGSHttp::TryDeserializeJsonObject(JsonString, JsonObject))
		{
			return false;
		}

		JsonObject->TryGetStringField(TEXT("message"), OutResponse.Message);
		JsonObject->TryGetStringField(TEXT("lobbyID"), OutResponse.LobbyID);

		return true;
	}

	bool TryParseLobbyList(const FString& JsonString, TArray<FCGSLobbyInfo>& OutLobbies)
	{
		OutLobbies.Reset();

		TArray<TSharedPtr<FJsonValue>> JsonArray;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (!FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			return false;
		}

		for (const TSharedPtr<FJsonValue>& Value : JsonArray)
		{
			if (!Value.IsValid() || !Value->AsObject().IsValid())
			{
				continue;
			}

			FCGSLobbyInfo LobbyInfo;
			if (ParseBasicLobbyInfo(Value->AsObject(), LobbyInfo))
			{
				OutLobbies.Add(MoveTemp(LobbyInfo));
			}
		}
		return true;
	}
}

