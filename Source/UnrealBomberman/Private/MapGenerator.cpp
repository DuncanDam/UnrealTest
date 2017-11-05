// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"


// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapGenerator::InitMap()
{
	MapData = FMapData(MapData.Width, MapData.Length);
}

void AMapGenerator::InitDestructibleWall(int32 Seed, float Frequency)
{
	FMath::SRandInit(Seed);

	for (int32 XIndex = 0; XIndex < MapData.Width; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < MapData.Length; YIndex++)
		{
			FMapBlock* Block = MapData.GetBlock(XIndex, YIndex);
			if (Block->BlockType != Normal)
				continue;

			float RandomNum = FMath::SRand() * 100;
			if (RandomNum < Frequency)
			{
				Block->BlockType = Destructible;
			}
		}
	}
}