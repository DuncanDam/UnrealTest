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
		FVector2D(Length - 1.f, Width - 1.f),
		FVector2D(Length - 1.f, Width - 2.f),
		FVector2D(Length - 2.f, Width - 1.f)
	};

	for (int32 i = 0; i < MapData.SizeWidth; i++)
	{
		for (int32 j = 0; j < MapData.SizeLength; j++)
		{
			FVector Loc = FVector(i * BlockSize * -1, j * BlockSize, 0);
			FMapBlock NewBlock = FMapBlock(i, j, Normal, Loc);

			//Set Indestructible Walls
			if ((i >= 1 && i <= Width - 2)
				&& (j >= 1 && j <= Length - 2)
				&& (i % 2 != 0 && j % 2 != 0)
				)
				NewBlock.BlockType = Indestructible;

			//Set Player safe zone
			if (PlayerSafeZoneList.Contains(FVector2D(i, j)))
				NewBlock.BlockType = PlayerSafeZone;

			//Set destructible wall base on random seed
			if (NewBlock.BlockType == Normal)
			{
				float RandomNum = FMath::SRand() * 100;
				if (RandomNum < Frequency)
				{
					NewBlock.BlockType = Destructible;
					float PowerUpChance = FMath::RandRange(0.f, 1.f) * 100;
					if (PowerUpChance <= 30.f)
					{
						NewBlock.bHavePowerUp = true;
						NewBlock.PowerUpType = TEnumAsByte<EPowerUpType>((uint8)FMath::RandRange(0, 3));

					}
				}
			}

			MapData.Blocks.Add(NewBlock);
		}
	}

	OnInitDone();
}

FVector AMapGenerator::GetBlockLocation(const FVector& CurrentLoc)
{
	FVector Result = CurrentLoc;
	Result.X = (int32)((Result.X - 50) / 100.f) * 100.f;
	Result.Y = (int32)((Result.Y + 50) / 100.f) * 100.f;
	return Result;
}