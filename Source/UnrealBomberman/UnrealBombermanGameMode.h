// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UnrealBombermanGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOMBERMAN_API AUnrealBombermanGameMode : public AGameMode
{
	GENERATED_BODY()

	FTimerHandle TimerHandle_DefaultTimer;
	
	UPROPERTY(config)
	int32 MatchTime = 180;

	int32 CurrentMatchTime;
	int32 NumberOfPlayers;

	TSubclassOf<class ABomberCharacter> BomberPawnClass;

	TSubclassOf<class AMapGenerator> MapGeneratorClass;

	class ABomberPlayerController* MainPC;

	class AMapGenerator* MapGenerator;
	int32 WinnerTeam = -1;

	AUnrealBombermanGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	
	bool IsMatchOver();

	void DefaultTimer();
		
	class ABomberPlayerController* GetMainPC();
	void SpawnBomber(int32 Team);

	void CheckForWinner();

public:
	UFUNCTION(BlueprintPure)
	class AMapGenerator* GetMapGenerator() const;

	UFUNCTION(BlueprintPure)
	float GetMatchTime() const;

	UFUNCTION(BlueprintPure)
	int32 GetWinnerTeam();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndMatch();

	UFUNCTION(BlueprintPure)
	TArray<class ABomberCharacter*> GetBombers();

	UFUNCTION(BlueprintPure)
	float GetBombersMaxDistToCenter();

	UFUNCTION(BlueprintPure)
	int32 GetTeamScore(int32 Team);
};
