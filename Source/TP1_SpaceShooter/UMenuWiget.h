// cpp
	#pragma once
	
	#include "CoreMinimal.h"
	#include "Blueprint/UserWidget.h"
	#include "UMenuWiget.generated.h"
	
	class UVerticalBox;
	class UButton;
	class UTextBlock;
	
	UCLASS()
	class TP1_SPACESHOOTER_API UMenuWidget : public UUserWidget
	{
	    GENERATED_BODY()
	
	public:
	    virtual void NativeConstruct() override;
	
	protected:
	    UPROPERTY(EditDefaultsOnly, Category="Menu")
	    FName PlayLevelName = FName("Level_Game");
	
	    UPROPERTY(EditDefaultsOnly, Category="Menu")
	    FString GithubUrl = TEXT("https://github.com/SystAmynna/UQAC_GameEngineUse_TP1_SpaceShooter");
	
	    UPROPERTY(EditDefaultsOnly, Category="Menu")
	    FText ControlsDescription = FText::FromString(
	        TEXT("Contrôles:\n- Se déplacer: axes `axeX` / `axeY`\n- Stabiliser: action `Stabilize`\n- Tirer: action `Shoot`")
	    );
	
	private:
	    UPROPERTY()
	    UVerticalBox* RootBox = nullptr;
	
	    UPROPERTY()
	    UButton* PlayButton = nullptr;
	
	    UPROPERTY()
	    UButton* GithubButton = nullptr;
	
	    UPROPERTY()
	    UTextBlock* ControlsText = nullptr;
	
	    UFUNCTION()
	    void OnPlayClicked();
	
	    UFUNCTION()
	    void OnGithubClicked();
	
	    void BuildUI();
	    UButton* MakeButton(const FText& Label);
	    UTextBlock* MakeText(const FText& Text, int32 Size = 24);
	};