// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHavePowerUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPowerUpType> PowerUpType;

	FMapBlock() {}
	FMapBlock(int32 NewX, int32 NewY, TEnumAsByte<EBlockType> NewType, FVector NewLoc) {
		X = NewX;
		Y = NewY;
		BlockType = NewType;
		Location = NewLoc;
	}
};

USTRUCT(BlueprintType)
struct FMapData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SizeLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlockSize;

	UPROPERTY(BlueprintReadOnly)
	TArray<FMapBlock> Blocks;

	FMapData() {}
	FMapData(int32 NewWidth, int32 NewLength, float NewBlockSize) {
		SizeWidth = NewWidth;
		SizeLength = NewLength;
		BlockSize = NewBlockSize;
		Width = SizeWidth * BlockSize;
		Length = SizeLength * BlockSize;
	}

	FMapBlock* GetBlock(int32 X, int32 Y) {
		int32 Index = X * SizeWidth + Y;
		return &Blocks[Index];
	}

	FVector BottomRight() {
		return FVector(BlockSize - Length, Width - BlockSize, BlockSize / 2.f);
	}

	FVector SpawnPoint(int32 TeamNo) {
		if (TeamNo == 0)
			return FVector(0.f, 0.f, 50.f);
		return FVector(0, Width - BlockSize, BlockSize / 2.f);
	}
};

UCLASS()
class UNREALBOMBERMAN_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMapData MapData;

	AMapGenerator();

	UFUNCTION(BlueprintCallable)
	void InitMap(int32 Width = 17, int32 Length = 17, int32 Seed = 0, float Frequency = 50.f, float BlockSize = 100.f);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInitDone();
};
