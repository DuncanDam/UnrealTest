// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BomberGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOMBERMAN_API UBomberGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	int32 Score[2];	
};
