// c++
#include "UMenuWiget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void UMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(true);

    BuildUI();

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UMenuWidget::OnPlayClicked);
    if (GithubButton)
        GithubButton->OnClicked.AddDynamic(this, &UMenuWidget::OnGithubClicked);
}

void UMenuWidget::BuildUI()
{
    if (!WidgetTree) return;

    auto* Canvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
    WidgetTree->RootWidget = Canvas;

    // Fond plein écran semi‑transparent pour vérifier l’affichage
    {
        UImage* Bg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Bg"));
        Bg->SetColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.6f));
        auto* BgSlot = Canvas->AddChildToCanvas(Bg);
        BgSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
        BgSlot->SetOffsets(FMargin(0.f));
        BgSlot->SetAlignment(FVector2D(0.f, 0.f));
    }

    RootBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootBox"));
    auto* Slot = Canvas->AddChildToCanvas(RootBox);
    Slot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
    Slot->SetAlignment(FVector2D(0.5f, 0.5f));
    Slot->SetAutoSize(true);

    // Titre
    {
        UTextBlock* Title = MakeText(FText::FromString(TEXT("Space Shooter")), 40);
        Title->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
        auto* VSlot = RootBox->AddChildToVerticalBox(Title);
        VSlot->SetPadding(FMargin(0.f, 30.f, 0.f, 20.f));
        VSlot->SetHorizontalAlignment(HAlign_Center);
    }

    // Bouton Jouer
    {
        PlayButton = MakeButton(FText::FromString(TEXT("Jouer")));
        auto* VSlot = RootBox->AddChildToVerticalBox(PlayButton);
        VSlot->SetPadding(FMargin(0.f, 10.f));
        VSlot->SetHorizontalAlignment(HAlign_Center);
    }

    // Texte contrôles
    {
        ControlsText = MakeText(ControlsDescription, 18);
        ControlsText->SetAutoWrapText(true);
        ControlsText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        auto* VSlot = RootBox->AddChildToVerticalBox(ControlsText);
        VSlot->SetPadding(FMargin(20.f, 20.f, 20.f, 10.f));
        VSlot->SetHorizontalAlignment(HAlign_Fill);
    }

    // Bouton GitHub
    {
        GithubButton = MakeButton(FText::FromString(TEXT("Ouvrir GitHub")));
        auto* VSlot = RootBox->AddChildToVerticalBox(GithubButton);
        VSlot->SetPadding(FMargin(0.f, 20.f, 0.f, 30.f));
        VSlot->SetHorizontalAlignment(HAlign_Center);
    }
}

UButton* UMenuWidget::MakeButton(const FText& Label)
{
    UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
    UTextBlock* Text = MakeText(Label, 28);
    Text->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
    Button->AddChild(Text);
    return Button;
}

UTextBlock* UMenuWidget::MakeText(const FText& Text, int32 Size)
{
    UTextBlock* TB = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    TB->SetText(Text);
    TB->SetJustification(ETextJustify::Center);
    FSlateFontInfo Font = TB->Font;
    Font.Size = Size;
    TB->SetFont(Font);
    return TB;
}

void UMenuWidget::OnPlayClicked()
{
    if (!GetWorld() || PlayLevelName.IsNone()) return;
    UGameplayStatics::OpenLevel(this, PlayLevelName);
}

void UMenuWidget::OnGithubClicked()
{
    if (GithubUrl.IsEmpty()) return;
    UKismetSystemLibrary::LaunchURL(GithubUrl);
}