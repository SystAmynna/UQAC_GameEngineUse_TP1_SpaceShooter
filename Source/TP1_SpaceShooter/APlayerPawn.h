#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "APlayerPawn.generated.h"

UCLASS()
class TP1_SPACESHOOTER_API APlayerPawn : public APawn
{
    GENERATED_BODY()

public:
    APlayerPawn(); // Constructeur
    
    // Méthode appelée au début du jeu
    virtual void BeginPlay() override;
    // Méthode appelée à chaque frame
    virtual void Tick(float DeltaTime) override;
    // Configuration des entrées
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Méthodes de mouvement
    void MoveX(float value);
    void MoveY(float Value);

protected:
    // Composants
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    // UCameraComponent* CameraComponent;

    // Variables de configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;
};