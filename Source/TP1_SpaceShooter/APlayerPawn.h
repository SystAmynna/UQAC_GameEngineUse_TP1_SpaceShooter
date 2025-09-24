#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "APlayerPawn.generated.h"

UCLASS()
class TP1_SPACESHOOTER_API APlayerPawn : public APawn
{
    GENERATED_BODY()

public:
    APlayerPawn(); // Constructeur

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPawnMovementComponent* MovementComponent;
    
    
    // Méthode appelée au début du jeu
    virtual void BeginPlay() override;
    // Méthode appelée à chaque frame
    virtual void Tick(float DeltaTime) override;
    // Configuration des entrées
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Méthodes de mouvement
    void MoveX(float value);
    void MoveY(float value);

    // Méthodes pour les actions
    
    void HoldStabilize();
    void ReleaseStabilize();
    void Stabilize();

    // bordures

    void CheckBorders();


    // Points de vie
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
    int32 VieMax = 3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Player")
    int32 VieCurrent = 0;

    // Perdre de la vie lors d'une collision avec un astéroïde
    UFUNCTION(BlueprintCallable, Category="Player")
    void LoseLife(int32 Amount = 1);

protected:
    // Composants
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    //UInputComponent* InputComponent;

    // Variables de configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxMoveSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Acceleration;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector2D Velocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector2D TopLeftBorder;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FVector2D BottomRightBorder;
    
    bool bHoldStabilize;


public:
    // Décalage si votre mesh ne « regarde » pas +X à yaw=0
    UPROPERTY(EditAnywhere, Category="Player|View")
    float FacingYawOffset = 0.f;

protected:
    // Met à jour l'orientation du mesh selon l'état des touches
    void UpdateFacingFromKeys();

private:
    bool bRightPressed = false;
    bool bLeftPressed  = false;
    bool bUpPressed    = false;
    bool bDownPressed  = false;

};