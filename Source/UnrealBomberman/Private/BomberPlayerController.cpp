// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberPlayerController.h"
#include "BomberCharacter.h"
#include "GameFramework/Pawn.h"

void ABomberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward_P1", this, &ABomberPlayerController::MoveForward_P1);
	InputComponent->BindAxis("MoveRight_P1", this, &ABomberPlayerController::MoveRight_P1);
	InputComponent->BindAction("PlaceBomb_P1", IE_Pressed, this, &ABomberPlayerController::OnPlaceBomb_P1);

	InputComponent->BindAxis("MoveForward_P2", this, &ABomberPlayerController::MoveForward_P2);
	InputComponent->BindAxis("MoveRight_P2", this, &ABomberPlayerController::MoveRight_P2);
	InputComponent->BindAction("PlaceBomb_P2", IE_Pressed, this, &ABomberPlayerController::OnPlaceBomb_P2);
}


void ABomberPlayerController::MoveForward_P1(float Val)
{
	MoveBomberForward(P1_Bomber, Val);
}

void ABomberPlayerController::MoveRight_P1(float Val)
{
	MoveBomberRight(P1_Bomber, Val);
}

void ABomberPlayerController::OnPlaceBomb_P1()
{
	if (P1_Bomber)
	{
		P1_Bomber->PlaceBomb();
	}
}

void ABomberPlayerController::MoveForward_P2(float Val)
{
	MoveBomberForward(P2_Bomber, Val);
}

void ABomberPlayerController::MoveRight_P2(float Val)
{
	MoveBomberRight(P2_Bomber, Val);
}

void ABomberPlayerController::OnPlaceBomb_P2()
{
	if (P2_Bomber)
	{
		P2_Bomber->PlaceBomb();
	}
}

void ABomberPlayerController::MoveBomberForward(ABomberCharacter* Bomber, float Val)
{
	if (Bomber == NULL)
		return;

	Bomber->AddMovementInput(FVector::ForwardVector, Val, true);
}

void ABomberPlayerController::MoveBomberRight(ABomberCharacter* Bomber, float Val)
{
	if (Bomber == NULL)
		return;

	Bomber->AddMovementInput(FVector::RightVector, Val, true);
}