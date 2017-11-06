// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BomberPlayerController.generated.h"

class ABomberCharacter;

/**
 * 
 */
UCLASS()
class UNREALBOMBERMAN_API ABomberPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void SetupInputComponent() override;
	
	void MoveForward_P1(float Val);
	void MoveRight_P1(float Val);
	void OnPlaceBomb_P1();
	
	void MoveForward_P2(float Val);
	void MoveRight_P2(float Val);
	void OnPlaceBomb_P2();

	void MoveBomberForward(ABomberCharacter* Bomber, float Val);
	void MoveBomberRight(ABomberCharacter* Bomber, float Val);

public:
	ABomberCharacter* P1_Bomber;
	ABomberCharacter* P2_Bomber;
};
