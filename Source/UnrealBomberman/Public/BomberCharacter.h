// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BomberCharacter.generated.h"

class ABomb;

UENUM(BlueprintType)
enum EPowerUpType
{
	ExtraBlast,
	ExtraBomb,
	ExtraSpeed,
	RemoteControl
};

UCLASS()
class UNREALBOMBERMAN_API ABomberCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	int32 MaxBombCount;
	int32 BombLifeSpan;
	float RemoteControlDuration;
	float CurrentRemoteControlTime;
	int32 BlastRange;
	bool bIsDying;
	int32 BombCount;

	UPROPERTY()
	TArray<ABomb*> PlacedBombs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABomb> BombClass;

	ABomberCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UPROPERTY(BlueprintReadOnly)
	int32 TeamNumber;
	
	void SetTeamNumber(int32 TeamNo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetTeamNumber();	

	int32 GetMaxBombCount();

	void PlaceBomb();
	
	bool CanPlaceBomb();

	UFUNCTION(BlueprintCallable)
	void GetPowerUp(EPowerUpType PowerType);

	void UseBomb(int Count);
};
