// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UENUM(BlueprintType)
enum EBlockType
{
	Normal,
	PlayerSafeZone,
	Indestructible,
	Destructible
};

USTRUCT(BlueprintType)
struct FMapBlock
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBlockType> BlockType;

	FMapBlock() {}
	FMapBlock(int32 NewX, int32 NewY, TEnumAsByte<EBlockType> NewType) {
		X = NewX;
		Y = NewY;
		BlockType = NewType;
	}
};

USTRUCT(BlueprintType)
struct FMapData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 17, ClampMax = 55))
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 17, ClampMax = 55))
	int32 Length;

	UPROPERTY(BlueprintReadOnly)
	TArray<FMapBlock> Blocks;

	FMapData() {}
	FMapData(int32 NewWidth, int32 NewLength) {
		Width = NewWidth;
		Length = NewLength;

		TArray<FVector2D> PlayerSafeZoneList = {
			FVector2D(0.f, 0.f),
			FVector2D(0.f, 1.f),
			FVector2D(1.f, 0.f),
			FVector2D(Width - 1.f, Length - 1.f),
			FVector2D(Width - 1.f, Length - 2.f),
			FVector2D(Width - 2.f, Length - 1.f)
		};

		for (int32 i = 0; i < Width; i++)
		{
			for (int32 j = 0; j < Length; j++)
			{
				EBlockType Type = Normal;

				if ((i >= 1 && i <= Width - 2)
					&& (j >= 1 && j <= Length - 2)
					&& (i % 2 != 0 && j % 2 != 0)
					)
					Type = Indestructible;

				if (PlayerSafeZoneList.Contains(FVector2D(i, j)))
					Type = PlayerSafeZone;

				Blocks.Add(FMapBlock(i, j, Type));
			}
		}
	}

	FMapBlock* GetBlock(int32 X, int32 Y) {
		int32 Index = X * Width + Y;
		return &Blocks[Index];
	}
};

UCLASS()
class UNREALBOMBERMAN_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMapData MapData;

	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitMap();

	UFUNCTION(BlueprintCallable)
	void InitDestructibleWall(int32 Seed, float Frequency);
};
