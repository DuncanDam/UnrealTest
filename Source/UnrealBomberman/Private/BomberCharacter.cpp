// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bomb.h"
#include "MapGenerator.h"

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
}

void ABomberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Count down remote control time
	if (CurrentRemoteControlTime > 0)
	{
		CurrentRemoteControlTime -= DeltaTime;
	}
}

float ABomberCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Damage <= 0 || bIsDying)
		return Damage;

	//Set character is dead
	bIsDying = true;
	return Damage;
}

void ABomberCharacter::SetTeamNumber(int32 TeamNo)
{
	//Set team number and trigger an event to change character color
	TeamNumber = TeamNo;
	OnSetTeamNumber();
}

int32 ABomberCharacter::GetMaxBombCount()
{
	return (CurrentRemoteControlTime > 0) ? 1 : MaxBombCount;
}

void ABomberCharacter::PlaceBomb()
{
	if (bIsDying)
		return;

	//Detonate bomb instead of place another one
	if (CanPlaceBomb())
	{
		//Convert Character location to block location
		FVector SpawnLocation = AMapGenerator::GetBlockLocation(GetActorLocation());
		FRotator SpawnRotation = FRotator(ForceInit);
		FTransform SpawnTranform = FTransform(SpawnRotation, SpawnLocation);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = this;
		SpawnInfo.ObjectFlags |= RF_Transient;

		//Spawn bomb on exact block character is standing in
		ABomb* NewBomb = GetWorld()->SpawnActor<ABomb>(BombClass, SpawnTranform, SpawnInfo);
		NewBomb->InitBomb(this);
		PlacedBombs.Add(NewBomb);
		UseBomb(1);
	}
	//Remote control bomb handle
	else if (CurrentRemoteControlTime > 0)
	{
		if (PlacedBombs.Num() > 0)
		{
			for (int32 i = 0; i < PlacedBombs.Num(); i++)
			{
				ABomb* Bomb = PlacedBombs[i];
				if (Bomb)
					Bomb->Explode();
			}
		}
	}
}

bool ABomberCharacter::CanPlaceBomb()
{
	if (BombClass == NULL)
		return false;
	if (BombCount >= GetMaxBombCount())
		return false;

	return true;
}

void ABomberCharacter::GetPowerUp(EPowerUpType PowerType)
{
	//Power up pickup handle
	switch (PowerType)
	{
		case ExtraBlast:
			BlastRange++;
			break;
		case ExtraBomb:
			MaxBombCount++;
			break;
		case ExtraSpeed:
			GetCharacterMovement()->MaxWalkSpeed += 100;
			break;
		case RemoteControl:
			CurrentRemoteControlTime = RemoteControlDuration;
			break;
	}
}

void ABomberCharacter::UseBomb(int Count)
{
	BombCount += Count;
	BombCount = FMath::Clamp(BombCount, 0, GetMaxBombCount());
}