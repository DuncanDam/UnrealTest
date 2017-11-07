// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "BomberCharacter.h"

float APickupItem::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDestroyed)
		return DamageAmount;
	
	bIsDestroyed = true;
	SetLifeSpan(0.01f);
	return DamageAmount;
}

void APickupItem::OnPickup(class ABomberCharacter* Bomber)
{
	if (Bomber == NULL)
		return;
	Bomber->GetPowerUp(PowerUp);
	SetLifeSpan(0.02f);
}