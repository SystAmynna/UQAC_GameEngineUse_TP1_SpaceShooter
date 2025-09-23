#include "APlayerPawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

APlayerPawn::APlayerPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Création des composants
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
    // Définir la collision par défaut
    MeshComponent->SetCollisionProfileName(TEXT("Pawn"));

    MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = RootComponent;

    MovementDirection = FVector2D::ZeroVector;
    Acceleration = 0.1f;
    MaxMoveSpeed = 600.0f;
    
}

void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    

    AddMovementInput(FVector(MovementDirection.X, MovementDirection.Y, 0.0f),  1.0f);

    if (bHoldStabilize) Stabilize();
    
    
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("axeX", this, &APlayerPawn::MoveX);
    PlayerInputComponent->BindAxis("axeY", this, &APlayerPawn::MoveY);

    PlayerInputComponent->BindAction("Stabilize", IE_Pressed, this, &APlayerPawn::HoldStabilize);
    PlayerInputComponent->BindAction("Stabilize", IE_Released, this, &APlayerPawn::ReleaseStabilize);
    
}




void APlayerPawn::MoveX(const float value)
{
    if (value == 0.0f) return;
    
    MovementDirection.X = MovementDirection.X + (value * Acceleration);

    if (MovementDirection.X > MaxMoveSpeed) MovementDirection.X = MaxMoveSpeed;
    if (MovementDirection.X < -MaxMoveSpeed) MovementDirection.X = -MaxMoveSpeed;
    
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "move X");
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "" + FString::SanitizeFloat(MovementDirection.X));
    
}

void APlayerPawn::MoveY(float value)
{
    
    if (value == 0.0f) return;
    
    MovementDirection.Y = MovementDirection.Y + (value * Acceleration);

    if (MovementDirection.Y > MaxMoveSpeed) MovementDirection.Y = MaxMoveSpeed;
    if (MovementDirection.Y < -MaxMoveSpeed) MovementDirection.Y = -MaxMoveSpeed;

    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "move Y");
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "" + FString::SanitizeFloat(MovementDirection.Y));
    
}

void APlayerPawn::HoldStabilize()
{
    bHoldStabilize = true;
}

void APlayerPawn::ReleaseStabilize()
{
    bHoldStabilize = false;
}

void APlayerPawn::Stabilize()
{
    
    if (MovementDirection.X > 0)
    {
        MovementDirection.X -= Acceleration;
        if (MovementDirection.X < Acceleration) MovementDirection.X = 0;
    }
    else if (MovementDirection.X < 0)
    {
        MovementDirection.X += Acceleration;
        if (MovementDirection.X > Acceleration) MovementDirection.X = 0;
    }

    if (MovementDirection.Y > 0)
    {
        MovementDirection.Y -= Acceleration;
        if (MovementDirection.Y < Acceleration) MovementDirection.Y = 0;
    }
    else if (MovementDirection.Y < 0)
    {
        MovementDirection.Y += Acceleration;
        if (MovementDirection.Y > Acceleration) MovementDirection.Y = 0;
    }
        
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Stabilize" + FString::SanitizeFloat(MovementDirection.X) + " - " + FString::SanitizeFloat(MovementDirection.Y));
}


