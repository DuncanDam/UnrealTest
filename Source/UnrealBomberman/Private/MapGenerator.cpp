// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
}

void AMapGenerator::InitMap(int32 Width, int32 Length, int32 Seed, float Frequency, float BlockSize)
{
	MapData = FMapData(Width, Length, BlockSize);
	FMath::SRandInit(Seed);

	TArray<FVector2D> PlayerSafeZoneList = {
		FVector2D(0.f, 0.f),
		FVector2D(0.f, 1.f),
		FVector2D(1.f, 0.f),
		FVector2D(Width - 1.f, Length - 1.f),
		FVector2D(Width - 1.f, Length - 2.f),
		FVector2D(Width - 2.f, Length - 1.f)
	};

	for (int32 i = 0; i < MapData.SizeWidth; i++)
	{
		for (int32 j = 0; j < MapData.SizeLength; j++)
		{
			EBlockType Type = Normal;

			//Set Indestructible Walls
			if ((i >= 1 && i <= Width - 2)
				&& (j >= 1 && j <= Length - 2)
				&& (i % 2 != 0 && j % 2 != 0)
				)
				Type = Indestructible;

			//Set Player safe zone
			if (PlayerSafeZoneList.Contains(FVector2D(i, j)))
				Type = PlayerSafeZone;

			//Set destructible wall base on random seed
			if (Type == Normal)
			{
				float RandomNum = FMath::SRand() * 100;
				if (RandomNum < Frequency)
				{
					Type = Destructible;
				}
			}

			FVector Loc = FVector(i * BlockSize * -1, j * BlockSize, 0);

			MapData.Blocks.Add(FMapBlock(i, j, Type, Loc));
		}
	}

	OnInitDone();
}