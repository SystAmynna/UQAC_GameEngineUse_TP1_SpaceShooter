// MyPlayerController.h
	    #pragma once
	    
	    #include "CoreMinimal.h"
	    #include "GameFramework/PlayerController.h"
	    #include "APlayerController.generated.h"
	    
	    UCLASS()
	    class TP1_SPACESHOOTER_API AMyPlayerController : public APlayerController
	    {
	        GENERATED_BODY()
	    
	    public:
	        AMyPlayerController();
	    
	    protected:
	        virtual void BeginPlay() override;
	        virtual void SetupInputComponent() override;
	    };