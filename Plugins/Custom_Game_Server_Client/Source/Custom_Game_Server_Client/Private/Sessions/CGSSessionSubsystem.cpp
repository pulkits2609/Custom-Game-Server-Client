#include "Sessions/CGSSessionSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Http/CGSHttpHelpers.h"
#include "Settings/CGSClientSettings.h"

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

void UCGSSessionSubsystem::Login(const FString& Username, const FString& Password){
	TSharedPtr<FJsonObject> JsonObject =
		MakeShared<FJsonObject>();

	JsonObject->SetStringField(
		TEXT("username"),
		Username);

	JsonObject->SetStringField(
		TEXT("password"),
		Password);

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

				return;
			}

			SessionID =
				JsonResponse->GetStringField(
					TEXT("sessionID"));

			SessionToken =
				JsonResponse->GetStringField(
					TEXT("token"));

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
		[]
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

				return;
			}

			const FString Username =
				JsonResponse->GetStringField(
					TEXT("username"));

			const FString PlayerName =
				JsonResponse->GetStringField(
					TEXT("playerName"));

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