#include "APlayerPawn.h"
#include "Components/InputComponent.h"

APlayerPawn::APlayerPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Création des composants
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Chargement et assignation d'un cube comme mesh par défaut
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMeshAsset.Object);
        MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        MeshComponent->SetWorldScale3D(FVector(0.5f)); // Taille du cube
    }
    
    // Définir la collision par défaut
    MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
}

void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveX", this, &APlayerPawn::MoveX);
    PlayerInputComponent->BindAxis("MoveY", this, &APlayerPawn::MoveY);
}

void APlayerPawn::MoveX(const float value)
{
    if (value == 0.0f) return;
    AddMovementInput(GetActorRightVector(), value * MoveSpeed);
    
}

void APlayerPawn::MoveY(float Value)
{
    if (Value == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Value * MoveSpeed);
    
}