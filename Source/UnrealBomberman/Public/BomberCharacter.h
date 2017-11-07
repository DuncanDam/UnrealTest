// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickupItem.h"
#include "BomberCharacter.generated.h"

class ABomb;

UCLASS()
class UNREALBOMBERMAN_API ABomberCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	int32 MaxBombCount;
	int32 CurrentBombCount;
	int32 BombLifeSpan;
	float RemoteControlDuration;
	float CurrentRemoteControlTime;
	int32 BlastRange;
	bool bIsDying;

	UPROPERTY()
	TArray<ABomb*> PlacedBombs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABomb> BombClass;

	ABomberCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly)
	int32 TeamNumber;
	
	void SetTeamNumber(int32 TeamNo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetTeamNumber();
	
	FVector CurrentBlockLocation();

	int32 GetMaxBombCount();

	void PlaceBomb();
	
	bool CanPlaceBomb();

	void GetPowerUp(EPowerUpType PowerType);

	void UseBomb(int32 Count);
};
