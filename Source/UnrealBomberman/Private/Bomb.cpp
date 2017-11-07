// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "../Public/BomberCharacter.h"
#include "UnrealBomberman.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ABomb::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount <= 0)
		return 0.f;

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

	GetWorldTimerManager().ClearTimer(TimerHandle_Explode);
	Bomber->PlacedBombs.Remove(this);
	Bomber->UseBomb(-1);

	UWorld* World = GetWorld();
	if (World)
	{
		TArray<AActor*> HitActors;		
		FQuat Rotator = GetActorRotation().Quaternion();
		FVector StartTrace = GetActorLocation();
		FVector EndTrace = FVector::ZeroVector;
		FCollisionShape TraceShape = FCollisionShape();
		TraceShape.SetBox(FVector(49.f, 49.f, 10.f));
		
		static FName BombTag = FName(TEXT("BombTrace"));
		
		FCollisionQueryParams TraceParams(BombTag, true, this);
		TraceParams.bTraceAsyncScene = true;

		//Trace in 4 direction
		for (int32 i = 0; i < 4; i++)
		{
			FVector Dir = FVector::ForwardVector;
			if (i == 1)
				Dir = -1 * FVector::ForwardVector;
			else if (i == 2)
				Dir = FVector::RightVector;
			else if (i == 3)
				Dir = -1 * FVector::RightVector;
			
			EndTrace = StartTrace + Dir * Bomber->BlastRange * 100.f;
			
			TArray<FHitResult> OutHits;
			if (World->SweepMultiByChannel(OutHits, StartTrace, EndTrace, Rotator, COLLISION_BOMB, TraceShape, TraceParams))
			{
				for (const FHitResult& Hit : OutHits)
				{
					if (Hit.GetActor() != NULL && Hit.GetActor()->bCanBeDamaged)
					{
						Hit.GetActor()->TakeDamage(1, FDamageEvent(), NULL, Bomber);
					}
				}
			}
		}
	}

	OnExplode(Bomber->BlastRange);
	SetLifeSpan(0.2f);

}