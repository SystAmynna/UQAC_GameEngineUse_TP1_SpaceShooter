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

    Velocity = FVector2D::ZeroVector;
    Acceleration = 0.1f;
    MaxMoveSpeed = 600.0f;

    TopLeftBorder = FVector2D(-1000.0f, -1000.0f);
    BottomRightBorder = FVector2D(1000.0f, 1000.0f);
    
}

void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    

    AddMovementInput(FVector(Velocity.X, Velocity.Y, 0.0f),  1.0f);

    FVector NewLocation = GetActorLocation();
    NewLocation.Z = 500.0f;
    SetActorLocation(NewLocation);

    if (bHoldStabilize) Stabilize();

    CheckBorders();
    
    
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
    
    Velocity.X = Velocity.X + (value * Acceleration);

    if (Velocity.X > MaxMoveSpeed) Velocity.X = MaxMoveSpeed;
    if (Velocity.X < -MaxMoveSpeed) Velocity.X = -MaxMoveSpeed;
    
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "move X");
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "" + FString::SanitizeFloat(MovementDirection.X));
    
}

void APlayerPawn::MoveY(float value)
{
    
    if (value == 0.0f) return;
    
    Velocity.Y = Velocity.Y + (value * Acceleration);

    if (Velocity.Y > MaxMoveSpeed) Velocity.Y = MaxMoveSpeed;
    if (Velocity.Y < -MaxMoveSpeed) Velocity.Y = -MaxMoveSpeed;

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
    
    if (Velocity.X > 0)
    {
        Velocity.X -= Acceleration;
        if (Velocity.X < Acceleration) Velocity.X = 0;
    }
    else if (Velocity.X < 0)
    {
        Velocity.X += Acceleration;
        if (Velocity.X > Acceleration) Velocity.X = 0;
    }

    if (Velocity.Y > 0)
    {
        Velocity.Y -= Acceleration;
        if (Velocity.Y < Acceleration) Velocity.Y = 0;
    }
    else if (Velocity.Y < 0)
    {
        Velocity.Y += Acceleration;
        if (Velocity.Y > Acceleration) Velocity.Y = 0;
    }
        
    // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Stabilize" + FString::SanitizeFloat(MovementDirection.X) + " - " + FString::SanitizeFloat(MovementDirection.Y));
}

void APlayerPawn::CheckBorders()
{
    float const X = GetActorLocation().X;
    float const Y = GetActorLocation().Y;

    float const TopBorder = TopLeftBorder.Y;
    float const BottomBorder = BottomRightBorder.Y;
    float const LeftBorder = TopLeftBorder.X;
    float const RightBorder = BottomRightBorder.X;

    if ((X < LeftBorder && Velocity.X < 0) || (X > RightBorder && Velocity.X > 0)) Velocity.X = Velocity.X * -1;
    
    if ((Y < TopBorder && Velocity.Y < 0) || (Y > BottomBorder && Velocity.Y > 0)) Velocity.Y = Velocity.Y * -1;
    
}
