#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CGSClientSettings.generated.h"

UCLASS(Config = Game, DefaultConfig)
class CUSTOM_GAME_SERVER_CLIENT_API UCGSClientSettings
	: public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UCGSClientSettings();

	virtual FName GetCategoryName() const override;
	virtual FText GetSectionText() const override;

	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Server")
	FString BaseUrl;

	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Server")
	FString RealtimeHost;

	UPROPERTY(
		Config,
		EditAnywhere,
		BlueprintReadOnly,
		Category="Server")
	int32 RealtimePort;
};