#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

class FJsonObject;

namespace CGSHttp
{
	// Base URL handling
	void SetBaseUrl(const FString& InBaseUrl);
	const FString& GetBaseUrl();
	FString BuildUrl(const FString& Path);

	// Request creation
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest(
		const FString& Path,
		const FString& Verb);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateJsonRequest(
		const FString& Path,
		const FString& Verb,
		const FString& JsonBody,
		const FString* AuthToken = nullptr);

	// Header helpers
	void SetJsonHeader(IHttpRequest& Request);
	void SetAuthHeader(IHttpRequest& Request, const FString& AuthToken);

	// JSON helpers
	FString SerializeJsonObject(const TSharedPtr<FJsonObject>& JsonObject);
	bool TryDeserializeJsonObject(const FString& JsonString, TSharedPtr<FJsonObject>& OutJsonObject);
}