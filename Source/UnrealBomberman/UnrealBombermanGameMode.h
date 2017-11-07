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
	TArray<class ABomberCharacter*> Bombers;

	TSubclassOf<class AMapGenerator> MapGeneratorClass;
	class AMapGenerator* MapGenerator;

	class ABomberPlayerController* MainPC;

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
};
