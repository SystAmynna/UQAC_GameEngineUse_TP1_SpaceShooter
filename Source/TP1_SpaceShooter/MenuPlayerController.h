// c++
	#pragma once
	
	#include "CoreMinimal.h"
	#include "GameFramework/PlayerController.h"
	#include "MenuPlayerController.generated.h"
	
	class UMenuWidget;
	
	UCLASS()
	class TP1_SPACESHOOTER_API AMenuPlayerController : public APlayerController
	{
	    GENERATED_BODY()
	
	public:
	    virtual void BeginPlay() override;
	
	private:
	    UPROPERTY()
	    UMenuWidget* Menu = nullptr;
	};