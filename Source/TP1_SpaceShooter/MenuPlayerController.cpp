// c++
#include "MenuPlayerController.h"
#include "UMenuWiget.h"
#include "Engine/Engine.h"

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsLocalController())
        return;

    bShowMouseCursor = true;

    // Mode input UI
    FInputModeUIOnly Mode;
    SetInputMode(Mode);

    // Crée et affiche sur l'écran du joueur local
    UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, UMenuWidget::StaticClass());
    if (!Menu)
    {
        UE_LOG(LogTemp, Error, TEXT("Menu: CreateWidget a échoué"));
        return;
    }

    Menu->SetVisibility(ESlateVisibility::Visible);
    Menu->AddToPlayerScreen(9999); // Z très haut pour passer au-dessus de tout

    // Focus (après AddToPlayerScreen)
    Mode.SetWidgetToFocus(Menu->TakeWidget());
    Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(Mode);

    // Logs de contrôle
    UE_LOG(LogTemp, Warning, TEXT("Menu IsInViewport=%s, Visibility=%d"),
        Menu->IsInViewport() ? TEXT("true") : TEXT("false"),
        (int32)Menu->GetVisibility());
}