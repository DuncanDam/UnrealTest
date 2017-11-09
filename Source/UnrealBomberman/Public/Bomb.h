// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

UCLASS()
class UNREALBOMBERMAN_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	ABomb();	
	bool bIsExploded;

	FTimerHandle TimerHandle_Explode;
	class ABomberCharacter* Bomber;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void InitBomb(class ABomberCharacter* NewBomber);
	void Explode();

	UFUNCTION(BlueprintImplementableEvent)
	void OnExplode(const TArray<float>& ExplosionDist);

};
