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
    FVector2D MovementDirection;

    bool bHoldStabilize;

};