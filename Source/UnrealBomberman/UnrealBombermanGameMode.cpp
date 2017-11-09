// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealBombermanGameMode.h"
#include "MapGenerator.h"
#include "BomberPlayerController.h"
#include "BomberCharacter.h"
#include "ConstructorHelpers.h"
#include "BomberGameInstance.h"

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

	bDelayedStart = true;
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
	MatchTime = 180;

	//Spawn map in waiting mode
	if (MapGenerator == NULL && MapGeneratorClass)
		MapGenerator = GetWorld()->SpawnActor<AMapGenerator>(MapGeneratorClass);
	
	MapGenerator->SetActorLocation(FVector::ZeroVector);
	if (MapGenerator)
		MapGenerator->InitMap(15.f, 15.f, FMath::RandRange(0, 100), 50.f);
}

void AUnrealBombermanGameMode::HandleMatchHasStarted()
{
	//Spawn bomber when match start
	Super::HandleMatchHasStarted();
	GetMainPC()->Bombers = TArray<ABomberCharacter*>();
	SpawnBomber(0);
	SpawnBomber(1);
}

void AUnrealBombermanGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	OnEndMatch();
}

void AUnrealBombermanGameMode::DefaultTimer()
{
	if (IsMatchInProgress())
	{
		CurrentMatchTime++;
		//Check match is over
		if (IsMatchOver())
		{
			//Check for winner and move match to end
			CheckForWinner();
			MainPC->DisableInput(MainPC);
			EndMatch();
		}
	}
}

bool AUnrealBombermanGameMode::IsMatchOver()
{
	//Check for match over
	if (CurrentMatchTime >= MatchTime)
		return true;

	for (ABomberCharacter* Bomber : GetMainPC()->Bombers)
	{
		if (Bomber->bIsDying)
		{
			return true;
		}
	}

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
	FVector StartLocation = MapGenerator->MapData.SpawnPoint(Team);

	FTransform SpawnTransform = FTransform(StartRotation, StartLocation);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;

	//Add bomber to list
	ABomberCharacter* Char = GetWorld()->SpawnActor<ABomberCharacter>(BomberPawnClass, SpawnTransform, SpawnInfo);
	if (Char)
	{
		Char->SetTeamNumber(Team);
		GetMainPC()->Bombers.Add(Char);
	}
}

void AUnrealBombermanGameMode::CheckForWinner()
{
	//Check Winner team
	WinnerTeam = -1;
	for (ABomberCharacter* Bomber : GetMainPC()->Bombers)
	{
		if (!Bomber->bIsDying)
		{
			WinnerTeam = Bomber->TeamNumber;
		}
	}

	//Record Team Score
	UBomberGameInstance* GI = Cast<UBomberGameInstance>(GetGameInstance());
	if (GI)
	{
		if (WinnerTeam != -1)
			GI->Score[WinnerTeam]++;
	}
}

AMapGenerator* AUnrealBombermanGameMode::GetMapGenerator() const
{
	return MapGenerator;
}

float AUnrealBombermanGameMode::GetMatchTime() const
{
	return MatchTime - CurrentMatchTime;
}

int32 AUnrealBombermanGameMode::GetWinnerTeam()
{
	return WinnerTeam;
}

TArray<ABomberCharacter*> AUnrealBombermanGameMode::GetBombers()
{
	if (GetMainPC())
		return GetMainPC()->Bombers;

	return TArray<ABomberCharacter*>();
}

float AUnrealBombermanGameMode::GetBombersMaxDistToCenter()
{
	if (GetMainPC() == NULL || GetMainPC()->Bombers.Num() < 1 || MapGenerator == NULL)
		return 0.f;
	float Dist1 = MapGenerator->MapData.MiddlePoint.X - GetMainPC()->Bombers[0]->GetActorLocation().X;
	float Dist12 = MapGenerator->MapData.MiddlePoint.Y - GetMainPC()->Bombers[0]->GetActorLocation().Y;
	float Dist2 = MapGenerator->MapData.MiddlePoint.X - GetMainPC()->Bombers[1]->GetActorLocation().X;
	float Dist22 = MapGenerator->MapData.MiddlePoint.Y - GetMainPC()->Bombers[1]->GetActorLocation().Y;
	Dist1 = FMath::Abs(Dist1);
	Dist12 = FMath::Abs(Dist12);
	Dist2 = FMath::Abs(Dist2);
	Dist22 = FMath::Abs(Dist22);
	
	return FMath::Max(FMath::Max(Dist1, Dist2), FMath::Max(Dist12, Dist22));
}

int32 AUnrealBombermanGameMode::GetTeamScore(int32 Team)
{
	UBomberGameInstance* GI = Cast<UBomberGameInstance>(GetGameInstance());
	if (GI)
		return GI->Score[Team];

	return 0;
}