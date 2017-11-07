// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum EPowerUpType
{
	ExtraBlast,
	ExtraBomb,
	ExtraSpeed,
	RemoteControl
};

UCLASS()
class UNREALBOMBERMAN_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPowerUpType> PowerUp;
	bool bIsDestroyed;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	void OnPickup(class ABomberCharacter* Bomber);
};
