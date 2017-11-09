// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "../Public/BomberCharacter.h"
#include "UnrealBomberman.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = false;
}

float ABomb::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0)
		return 0.f;

	//Explode on being damaged;
	Explode();	
	return 0.f;
}

void ABomb::InitBomb(ABomberCharacter* NewBomber)
{
	Bomber = NewBomber;
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ABomb::Explode, Bomber->BombLifeSpan);	
}

void ABomb::Explode()
{
	if (bIsExploded)
		return;

	bIsExploded = true;

	//Clear timer
	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);

	//Remove bomb place on character
	Bomber->PlacedBombs.Remove(this);
	Bomber->UseBomb(-1);

	TArray<float> ExplosionDistance = { 50.f, 50.f, 50.f, 50.f };

	UWorld* World = GetWorld();
	if (World)
	{
		TArray<AActor*> HitActors;		
		FQuat Rotator = GetActorRotation().Quaternion();
		FVector StartTrace = GetActorLocation();
		FVector EndTrace = FVector::ZeroVector;
		
		static FName BombTag = FName(TEXT("BombTrace"));		
		FCollisionQueryParams TraceParams(BombTag, true, this);
		TraceParams.bTraceAsyncScene = true;

		//Do line trace in 4 direction
		for (int32 i = 0; i < 4; i++)
		{
			FVector Dir = FVector::ForwardVector;
			if (i == 1)
				Dir = -1 * FVector::ForwardVector;
			else if (i == 2)
				Dir = FVector::RightVector;
			else if (i == 3)
				Dir = -1 * FVector::RightVector;
			
			float BombRange = Bomber->BlastRange * 100.f + 49.f;
			EndTrace = StartTrace + Dir * BombRange;

			TArray<FHitResult> OutHits;

			bool bIsBlockingHit = false;

			//Cast a line trace to check for any object in bomb blast
			//Stop trace at first blocking hit
			bIsBlockingHit = World->LineTraceMultiByChannel(OutHits, StartTrace, EndTrace, COLLISION_BOMB, TraceParams);
			
			//Run through all hit result to check for valid object to deal damage
			for (const FHitResult& Hit : OutHits)
			{
				//Deal damage to actor
				if (Hit.GetActor() != NULL && Hit.GetActor()->bCanBeDamaged)
				{
					Hit.GetActor()->TakeDamage(1, FDamageEvent(), NULL, Bomber);
				}

				//Check if hitting wall then calculate for explosion effect length
				if (Hit.IsValidBlockingHit())
				{
					bIsBlockingHit = true;
					float Dist = (Hit.ImpactPoint - StartTrace).Size();
					if (Dist > ExplosionDistance[i])
					{
						ExplosionDistance[i] = Dist;
					}
				}
			}
			
			//If no block hit display bomb explosion at maximum range
			if (!bIsBlockingHit)
			{
				ExplosionDistance[i] = BombRange;
			}
		}
	}

	//Call event on blueprint to spawn effect
	OnExplode(ExplosionDistance);
	
	//Destroy after a brief time
	SetLifeSpan(0.1f);

}