// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberPlayerController.h"
#include "BomberCharacter.h"
#include "GameFramework/Pawn.h"

ABomberPlayerController::ABomberPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

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

void ABomberPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABomberPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Bombers = TArray<ABomberCharacter*>();
}

void ABomberPlayerController::MoveForward_P1(float Val)
{
	if (Bombers.Num() > 0)
		MoveBomberForward(Bombers[0], Val);
}

void ABomberPlayerController::MoveRight_P1(float Val)
{
	if (Bombers.Num() > 0)
		MoveBomberRight(Bombers[0], Val);
}

void ABomberPlayerController::OnPlaceBomb_P1()
{
	if (Bombers.Num() > 0 && Bombers[0])
		Bombers[0]->PlaceBomb();
}

void ABomberPlayerController::MoveForward_P2(float Val)
{
	if (Bombers.Num() > 1 && Bombers[1])
		MoveBomberForward(Bombers[1], Val);
}

void ABomberPlayerController::MoveRight_P2(float Val)
{
	if (Bombers.Num() > 1 && Bombers[1])
		MoveBomberRight(Bombers[1], Val);
}

void ABomberPlayerController::OnPlaceBomb_P2()
{
	if (Bombers.Num() > 1 && Bombers[1])
		Bombers[1]->PlaceBomb();
}

void ABomberPlayerController::MoveBomberForward(ABomberCharacter* Bomber, float Val)
{
	if (Bomber == NULL || Bomber->bIsDying)
		return;

	Bomber->AddMovementInput(FVector::ForwardVector, Val, true);
}

void ABomberPlayerController::MoveBomberRight(ABomberCharacter* Bomber, float Val)
{
	if (Bomber == NULL || Bomber->bIsDying)
		return;

	Bomber->AddMovementInput(FVector::RightVector, Val, true);
}