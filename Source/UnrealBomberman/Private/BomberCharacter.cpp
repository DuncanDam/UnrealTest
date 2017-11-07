// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bomb.h"

ABomberCharacter::ABomberCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bRunPhysicsWithNoController = true;
	MaxBombCount = 1;
	BombLifeSpan = 3;
	BlastRange = 1;
	RemoteControlDuration = 10.f;
	CurrentRemoteControlTime = 0.f;
}

void ABomberCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlacedBombs = TArray<ABomb*>();
	PlacedBombs.Empty();
}

void ABomberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentRemoteControlTime > 0)
	{
		CurrentRemoteControlTime -= DeltaTime;
	}
}

void ABomberCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABomberCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDying)
		return Damage;

	bIsDying = true;
	return Damage;
}

void ABomberCharacter::SetTeamNumber(int32 TeamNo)
{
	TeamNumber = TeamNo;
	OnSetTeamNumber();
}

FVector ABomberCharacter::CurrentBlockLocation()
{
	FVector Result = GetActorLocation();
	Result.X = (int32)((Result.X - 50) / 100.f) * 100.f;
	Result.Y = (int32)((Result.Y + 50) / 100.f) * 100.f;
	return Result;
}

int32 ABomberCharacter::GetMaxBombCount()
{
	return (CurrentRemoteControlTime > 0) ? 1 : MaxBombCount;
}

void ABomberCharacter::PlaceBomb()
{
	//Detonate bomb instead of place another one
	if (CurrentRemoteControlTime > 0 && CurrentBombCount > 0)
	{
		if (PlacedBombs.Num() > 0)
		{
			for (int32 i = 0; i <= PlacedBombs.Num(); i++)
			{
				ABomb* Bomb = PlacedBombs[i];
				if (Bomb)
					Bomb->Explode();
			}
		}		
	}
	else if (CanPlaceBomb())
	{
		FVector SpawnLocation = CurrentBlockLocation();
		FRotator SpawnRotation = FRotator(ForceInit);
		FTransform SpawnTranform = FTransform(SpawnRotation, SpawnLocation);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = this;
		SpawnInfo.ObjectFlags |= RF_Transient;

		ABomb* NewBomb = GetWorld()->SpawnActor<ABomb>(BombClass, SpawnTranform, SpawnInfo);
		NewBomb->InitBomb(this);
		PlacedBombs.Add(NewBomb);
		UseBomb(1);
	}
}

bool ABomberCharacter::CanPlaceBomb()
{
	if (BombClass == NULL)
		return false;
	if (PlacedBombs.Num() >= GetMaxBombCount())
		return false;
	if (CurrentBombCount >= GetMaxBombCount())
		return false;
	return true;
}

void ABomberCharacter::GetPowerUp(EPowerUpType PowerType)
{
	switch (PowerType)
	{
		case ExtraBlast:
			BlastRange++;
			break;
		case ExtraBomb:
			MaxBombCount++;
			break;
		case ExtraSpeed:
			GetCharacterMovement()->MaxWalkSpeed += 50;
			break;
		case RemoteControl:
			CurrentRemoteControlTime = RemoteControlDuration;
			break;
	}
}

void ABomberCharacter::UseBomb(int32 Count)
{
	CurrentBombCount += Count;
	CurrentBombCount = FMath::Min(CurrentBombCount, GetMaxBombCount());
}