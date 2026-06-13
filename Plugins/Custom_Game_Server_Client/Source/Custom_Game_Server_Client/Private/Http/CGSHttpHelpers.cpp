#include "Http/CGSHttpHelpers.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"

namespace CGSHttp
{
	static FString GBaseUrl = TEXT("http://localhost:8080");

	void SetBaseUrl(const FString& InBaseUrl)
	{
		GBaseUrl = InBaseUrl;
	}

	const FString& GetBaseUrl()
	{
		return GBaseUrl;
	}

	FString BuildUrl(const FString& Path)
	{
		if (Path.StartsWith(TEXT("/")))
		{
			return GBaseUrl + Path;
		}

		return GBaseUrl + TEXT("/") + Path;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest(
		const FString& Path,
		const FString& Verb)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(BuildUrl(Path));
		Request->SetVerb(Verb);
		return Request;
	}

	void SetJsonHeader(IHttpRequest& Request)
	{
		Request.SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	}

	void SetAuthHeader(IHttpRequest& Request, const FString& AuthToken)
	{
		Request.SetHeader(
			TEXT("Authorization"),
			TEXT("Bearer ") + AuthToken);
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateJsonRequest(
		const FString& Path,
		const FString& Verb,
		const FString& JsonBody,
		const FString* AuthToken)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = CreateRequest(Path, Verb);

		SetJsonHeader(Request.Get());

		if (AuthToken && !AuthToken->IsEmpty())
		{
			SetAuthHeader(Request.Get(), *AuthToken);
		}

		Request->SetContentAsString(JsonBody);
		return Request;
	}

	FString SerializeJsonObject(const TSharedPtr<FJsonObject>& JsonObject)
	{
		if (!JsonObject.IsValid())
		{
			return FString();
		}

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
		return JsonString;
	}

	bool TryDeserializeJsonObject(const FString& JsonString, TSharedPtr<FJsonObject>& OutJsonObject)
	{
		OutJsonObject.Reset();

		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		return FJsonSerializer::Deserialize(Reader, OutJsonObject) && OutJsonObject.IsValid();
	}
}