// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealBombermanGameMode.h"
#include "MapGenerator.h"
#include "BomberPlayerController.h"
#include "BomberCharacter.h"
#include "ConstructorHelpers.h"

AUnrealBombermanGameMode::AUnrealBombermanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ABomberCharacter> BomberPawnBPClass(TEXT("/Game/Blueprints/BomberPawn"));
	if (BomberPawnBPClass.Class != NULL)
	{
		BomberPawnClass = BomberPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AMapGenerator> MapGeneratorBPClass(TEXT("/Game/Blueprints/MapGenerator"));
	if (MapGeneratorBPClass.Class != NULL)
	{
		MapGeneratorClass = MapGeneratorBPClass.Class;
	}
}

void AUnrealBombermanGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AUnrealBombermanGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);	
}

void AUnrealBombermanGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	CurrentMatchTime = 0;
	if (MapGenerator == NULL && MapGeneratorClass)
		MapGenerator = GetWorld()->SpawnActor<AMapGenerator>(MapGeneratorClass);
	
	MapGenerator->SetActorLocation(FVector::ZeroVector);
	if (MapGenerator)
		MapGenerator->InitMap();
}

void AUnrealBombermanGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	Bombers = TArray<ABomberCharacter*>();
	SpawnBomber(0);
	SpawnBomber(1);
}

void AUnrealBombermanGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void AUnrealBombermanGameMode::DefaultTimer()
{
	if (IsMatchInProgress())
	{
		CurrentMatchTime++;
		if (IsMatchOver())
		{
			EndMatch();
		}
	}
}

bool AUnrealBombermanGameMode::IsMatchOver()
{
	if (CurrentMatchTime >= MatchTime)
		return true;
	return false;
}

ABomberPlayerController* AUnrealBombermanGameMode::GetMainPC()
{
	if (MainPC != NULL)
		return MainPC;

	UWorld* World = GetWorld();
	if (World)
	{
		MainPC = Cast<ABomberPlayerController>(World->GetFirstPlayerController());
	}

	return MainPC;
}

void AUnrealBombermanGameMode::SpawnBomber(int32 Team)
{
	FRotator StartRotation(ForceInit);
	FVector StartLocation = FVector(0.f, 0.f, 50.f);

	if (Team == 1)
		StartLocation = MapGenerator->MapData.BottomRight();

	FTransform SpawnTransform = FTransform(StartRotation, StartLocation);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;

	ABomberCharacter* Char = GetWorld()->SpawnActor<ABomberCharacter>(BomberPawnClass, SpawnTransform, SpawnInfo);
	if (Char)
	{
		Char->SetTeamNumber(Team);
		Bombers.Add(Char);
	}
}