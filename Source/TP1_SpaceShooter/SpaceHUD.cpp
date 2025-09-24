// SpaceHUD.cpp
#include "SpaceHUD.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "APlayerPawn.h" // Ton pawn

void ASpaceHUD::DrawHUD()
{
	Super::DrawHUD();

	// Récupérer le pawn
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PC->GetPawn());
	if (!PlayerPawn) return;

	// Vérifier les valeurs de VieCurrent et VieMax
	FString HealthString;
	FLinearColor Color = FLinearColor::Gray; // défaut si valeurs invalides
	if (PlayerPawn->VieCurrent >= 0 && PlayerPawn->VieMax > 0)
	{
		HealthString = FString::Printf(TEXT("%d / %d"), 
					 PlayerPawn->VieCurrent,
					 PlayerPawn->VieMax);

		const float HealthPct = FMath::Clamp(
			static_cast<float>(PlayerPawn->VieCurrent) / static_cast<float>(PlayerPawn->VieMax),
			0.0f, 1.0f);

		if (HealthPct > 0.67f)
			Color = FLinearColor::Green;
		else if (HealthPct > 0.34f)
			Color = FLinearColor::Yellow;
		else
			Color = FLinearColor::Red;
		
	}
	else
	{
		HealthString = TEXT("N/A");
	}

	// Afficher en haut à gauche
	FVector2D ScreenPos(50, 50); // position en pixels
	float Scale = 1.5f;
	const float Margin = 50.f;

	UFont* DefaultFont = GEngine->GetLargeFont(); // Exemple d'utilisation d'une police par défaut
	DrawText(HealthString, Color, ScreenPos.X, ScreenPos.Y, DefaultFont, Scale, false);

	// --- AFFICHAGE DU SCORE (en haut à droite) ---
	FString ScoreString = FString::Printf(TEXT("Score: %d"), PlayerPawn->Score);

	// Calculer la taille du texte pour l'aligner à droite
	float TextWidth, TextHeight;
	GetTextSize(ScoreString, TextWidth, TextHeight, DefaultFont, Scale);

	// Position en haut à droite
	FVector2D ScorePos(Canvas->SizeX - TextWidth - Margin, Margin);

	DrawText(ScoreString, FLinearColor::White, ScorePos.X, ScorePos.Y, DefaultFont, Scale, false);

	
}
