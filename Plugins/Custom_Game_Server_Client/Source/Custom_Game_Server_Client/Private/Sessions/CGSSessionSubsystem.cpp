#include "Sessions/CGSSessionSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Http/CGSHttpHelpers.h"
#include "Settings/CGSClientSettings.h"
#include "Realtime/CGSRealtimeSubsystem.h"

//all of the above includes are classes being provided by dependencies : 
//http , jsonUtilities, json which were added to build.cs

void UCGSSessionSubsystem::Initialize(
    FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("CGS Session Subsystem Initialized"));

	const UCGSClientSettings* Settings =
		GetDefault<UCGSClientSettings>();

	CGSHttp::SetBaseUrl(
		Settings->BaseUrl);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("CGS Base URL: %s"),
		*Settings->BaseUrl);
    
}

void UCGSSessionSubsystem::Deinitialize()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("CGS Session Subsystem Deinitialized"));

    Super::Deinitialize();
}

void UCGSSessionSubsystem::Login(const FString& InUsername,const FString& InPassword){
	TSharedPtr<FJsonObject> JsonObject =
		MakeShared<FJsonObject>();

	JsonObject->SetStringField(
		TEXT("username"),
		InUsername);

	JsonObject->SetStringField(
		TEXT("password"),
		InPassword);

	const FString JsonBody =
		CGSHttp::SerializeJsonObject(JsonObject);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateJsonRequest(
			TEXT("/session/login"),
			TEXT("POST"),
			JsonBody);

	Request->OnProcessRequestComplete().BindLambda(
		[this]
		(
			FHttpRequestPtr Request,
			FHttpResponsePtr Response,
			bool bWasSuccessful
		)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Login Request Failed"));
				
				OnLoginFailed.Broadcast(TEXT("Login Request Failed"));

				return;
			}

			TSharedPtr<FJsonObject> JsonResponse;

			if (!CGSHttp::TryDeserializeJsonObject(
				Response->GetContentAsString(),
				JsonResponse))
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Failed To Parse Login Response"));
				
				OnLoginFailed.Broadcast(TEXT("Failed To Parse Login Response"));
				
				return;
			}

			SessionID =
				JsonResponse->GetStringField(
					TEXT("sessionID"));

			SessionToken =
				JsonResponse->GetStringField(
					TEXT("token"));
			
			FCGSLoginResponse LoginResponse;
			LoginResponse.SessionID = SessionID;
			LoginResponse.Token = SessionToken;

			OnLoginSuccess.Broadcast(LoginResponse);
			if (UGameInstance* GI = GetGameInstance())
			{
				if (UCGSRealtimeSubsystem* Realtime =
					GI->GetSubsystem<UCGSRealtimeSubsystem>())
				{
					Realtime->Connect();
				}
			}

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Logged In"));

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("SessionID: %s"),
				*SessionID);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Token: %s"),
				*SessionToken);

			FetchSession();
		});

	Request->ProcessRequest();
}

void UCGSSessionSubsystem::Logout()
{
	if (!IsLoggedIn())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Cannot Logout - Not Logged In"));

		return;
	}

	TSharedPtr<FJsonObject> JsonObject =
		MakeShared<FJsonObject>();

	JsonObject->SetStringField(
		TEXT("token"),
		SessionToken);

	const FString JsonBody =
		CGSHttp::SerializeJsonObject(JsonObject);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateJsonRequest(
			TEXT("/session/logout"),
			TEXT("POST"),
			JsonBody);

	Request->OnProcessRequestComplete().BindLambda(
		[this]
		(
			FHttpRequestPtr Request,
			FHttpResponsePtr Response,
			bool bWasSuccessful
		)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Logout Failed"));
				return;
			}

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s"),
				*Response->GetContentAsString());

			SessionID.Empty();
			SessionToken.Empty();
			OnLogoutSuccess.Broadcast();
		});

	Request->ProcessRequest();
}

void UCGSSessionSubsystem::FetchSession()
{
	if (!IsLoggedIn())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Cannot Fetch Session - Not Logged In"));

		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		CGSHttp::CreateRequest(
			FString::Printf(
				TEXT("/session/fetch/%s"),
				*SessionID),
			TEXT("GET"));

	CGSHttp::SetAuthHeader(
		*Request,
		SessionToken);

	Request->OnProcessRequestComplete().BindLambda(
		[this]
		(
			FHttpRequestPtr Request,
			FHttpResponsePtr Response,
			bool bWasSuccessful
		)
		{
			if (!bWasSuccessful || !Response.IsValid())
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Fetch Session Failed"));
				OnSessionFetchFailed.Broadcast(TEXT("Fetch Session Failed"));
				return;
			}

			TSharedPtr<FJsonObject> JsonResponse;

			if (!CGSHttp::TryDeserializeJsonObject(
				Response->GetContentAsString(),
				JsonResponse))
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Failed To Parse Session Response"));
				OnSessionFetchFailed.Broadcast(TEXT("Failed To Parse Session Response"));
				return;
			}

			Username = JsonResponse->GetStringField(TEXT("username"));

			PlayerName = JsonResponse->GetStringField(TEXT("playerName"));
			
			FCGSSessionInfo SessionInfo;

			SessionInfo.SessionID = SessionID;
			SessionInfo.Username = Username;
			SessionInfo.PlayerName = PlayerName;

			OnSessionFetched.Broadcast(SessionInfo);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Username: %s"),
				*Username);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("PlayerName: %s"),
				*PlayerName);
		});

	Request->ProcessRequest();
}

const FString& UCGSSessionSubsystem::GetSessionID() const
{
    return SessionID;
}

const FString& UCGSSessionSubsystem::GetSessionToken() const
{
    return SessionToken;
}

bool UCGSSessionSubsystem::IsLoggedIn() const
{
    return !SessionToken.IsEmpty();
}

const FString& UCGSSessionSubsystem::GetUsername() const
{
    return Username;
}

const FString& UCGSSessionSubsystem::GetPlayerName() const
{
    return PlayerName;
}