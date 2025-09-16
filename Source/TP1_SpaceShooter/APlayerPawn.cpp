#include "APlayerPawn.h"
#include "Components/InputComponent.h"

APlayerPawn::APlayerPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Création des composants
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
    // Définir la collision par défaut
    MeshComponent->SetCollisionProfileName(TEXT("Pawn"));

    left = false;
    right = false;
    up = false;
    down = false;
    
    
}

void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float v = Acceleration * DeltaTime;

    if (left && !right) MoveX(-v);
    else if (!left && right) MoveX(v);

    if (up && !down) MoveY(-v);
    else if (!up && down) MoveY(v);

    // Appliquer le mouvement à l'acteur
    if (!MovementDirection.IsZero())
    {
        // Convertir le FVector2D en FVector (X,Y,0)
        FVector Movement(MovementDirection.X, MovementDirection.Y, 0.0f);
        
        // Appliquer le mouvement à l'acteur
        AddActorWorldOffset(Movement * DeltaTime);
        
        // Collisions world
        // TODO : Gérer les collisions avec le monde
    }

    
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //PlayerInputComponent->BindBindAction()
}

void APlayerPawn::HoldLeft()
{
    left = true;
}
void APlayerPawn::HoldRight()
{
    right = true;
}
void APlayerPawn::HoldUp()
{
    up = true;
}
void APlayerPawn::HoldDown()
{
    down = true;
}
void APlayerPawn::ReleaseLeft()
{
    left = false;
}
void APlayerPawn::ReleaseRight()
{
    right = false;
}
void APlayerPawn::ReleaseUp()
{
    up = false;
}
void APlayerPawn::ReleaseDown()
{
    down = false;
}





void APlayerPawn::MoveX(const float value)
{
    if (value == 0.0f) return;
    
    MovementDirection.X += value;

    if (MovementDirection.X > MaxMoveSpeed) MovementDirection.X = MaxMoveSpeed;
    if (MovementDirection.X < -MaxMoveSpeed) MovementDirection.X = -MaxMoveSpeed;
    
}

void APlayerPawn::MoveY(float value)
{
    if (value == 0.0f) return;
    
    MovementDirection.Y += value;

    if (MovementDirection.Y > MaxMoveSpeed) MovementDirection.Y = MaxMoveSpeed;
    if (MovementDirection.Y < -MaxMoveSpeed) MovementDirection.Y = -MaxMoveSpeed;
    
}