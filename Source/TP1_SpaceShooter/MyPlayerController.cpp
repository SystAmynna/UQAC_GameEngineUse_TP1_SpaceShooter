// MyPlayerController.cpp
#include "MyPlayerController.h"
#include "APlayerPawn.h"

AMyPlayerController::AMyPlayerController()
{
    bShowMouseCursor = true;
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // S'assurer que le Pawn peut recevoir des entrées
    if (GetPawn())
    {
        GetPawn()->EnableInput(this);
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    // Les contrôles sont déjà configurés dans le Pawn,
    // ce controller va juste s'assurer que le Pawn les reçoit
}