#include "APlayerPawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SpaceProjectile.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

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

    VieMax = 3;
    
    
}

void APlayerPawn::BeginPlay()
{
    Super::BeginPlay();
    VieCurrent = VieMax;

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Utiliser les fleches pour bouger");
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Espace pour stabiliser");
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "E pour tirer");
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Menu principal pas finit !");
    
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

    // Orientation du mesh basée sur les touches (pas la vélocité)
    UpdateFacingFromKeys();
    
    
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("axeX", this, &APlayerPawn::MoveX);
    PlayerInputComponent->BindAxis("axeY", this, &APlayerPawn::MoveY);

    PlayerInputComponent->BindAction("Stabilize", IE_Pressed, this, &APlayerPawn::HoldStabilize);
    PlayerInputComponent->BindAction("Stabilize", IE_Released, this, &APlayerPawn::ReleaseStabilize);

    PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::OnShoot);
    PlayerInputComponent->BindAction("Shoot", IE_Released, this, &APlayerPawn::OffShoot);
    
}

void APlayerPawn::UpdateFacingFromKeys()
{
    // XOR par axe : si touches opposées enfoncées, on ignore l’axe
    int32 dx = (bRightPressed != bLeftPressed) ? (bRightPressed ? 1 : -1) : 0;
    int32 dy = (bUpPressed    != bDownPressed) ? (bUpPressed    ? 1 : -1) : 0;

    // Si aucune direction nette, on n’oriente pas (ignore)
    if (dx == 0 && dy == 0) return;

    // Yaw en degrés (mesh supposé orienté +X à yaw=0)
    float Yaw = FMath::RadiansToDegrees(FMath::Atan2((float)dy, (float)dx)) + FacingYawOffset;

    //ajouter 90 degres
    // Yaw += 90.f;

    // Orientation uniquement du mesh (pas de l’actor)
    if (MeshComponent)
    {
        MeshComponent->SetRelativeRotation(FRotator(0.f, Yaw +90.0f, 0.f));
        CurrentYaw = Yaw; // mémorise l’orientation du joueur
    }
}


void APlayerPawn::OnShoot()
{
    bIsShooting = true;

    if (!GetWorldTimerManager().IsTimerActive(FireTimerHandle))
    {
        // Tir immédiat puis cadence régulière
        FireOnce();
        GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerPawn::FireOnce, FireInterval, true);
    }
}

void APlayerPawn::OffShoot()
{
    bIsShooting = false;
    GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void APlayerPawn::FireOnce()
{
    if (!bIsShooting || !ProjectileClass || !MeshComponent) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // On récupère la rotation et la position actuelles du mesh dans le monde
    const FRotator _MeshWorldRotation = MeshComponent->GetComponentRotation();
    const FRotator MeshWorldRotation = _MeshWorldRotation + FRotator(0.0f,  -90.0f, 0.0f); // on ignore le pitch et le roll
    const FVector MeshWorldLocation = MeshComponent->GetComponentLocation();

    // La direction du tir est le vecteur "avant" du mesh
    const FVector FireDirection = MeshWorldRotation.Vector();

    // La position de départ du projectile est calculée à partir de la position du mesh,
    // en ajoutant un décalage (MuzzleLocalOffset) qui est tourné selon l'orientation du mesh.
    FVector MuzzleLocation = MeshWorldLocation + MeshWorldRotation.RotateVector(MuzzleLocalOffset);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = this;
    Params.Instigator = this;

    // On fait apparaître le projectile avec la bonne rotation
    ASpaceProjectile* Proj = World->SpawnActor<ASpaceProjectile>(ProjectileClass, MuzzleLocation, MeshWorldRotation, Params);
    if (Proj)
    {
        Proj->InitVelocity(FireDirection, ProjectileSpeed);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Failed to spawn projectile."));
    }
}



void APlayerPawn::MoveX(const float value)
{
    bRightPressed = false;
    bLeftPressed = false;
    if (value == 0.0f) return;

    if (value > 0.0f) bRightPressed = true;
    else if (value < 0.0f) bLeftPressed = true;
    
    Velocity.X = Velocity.X + (value * Acceleration);

    if (Velocity.X > MaxMoveSpeed) Velocity.X = MaxMoveSpeed;
    if (Velocity.X < -MaxMoveSpeed) Velocity.X = -MaxMoveSpeed;
    
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "move X");
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "" + FString::SanitizeFloat(MovementDirection.X));
    
}

void APlayerPawn::MoveY(float value)
{

    bUpPressed = false;
    bDownPressed = false;
    if (value == 0.0f) return;
    if (value > 0.0f) bUpPressed = true;
    else if (value < 0.0f) bDownPressed = true;
    
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
    bRightPressed = false;
    bLeftPressed = false;
    bUpPressed = false;
    bDownPressed = false;
    
    if (Velocity.X > 0)
    {
        Velocity.X -= Acceleration;
        bLeftPressed = true;
        if (Velocity.X < Acceleration) Velocity.X = 0;
    }
    else if (Velocity.X < 0)
    {
        Velocity.X += Acceleration;
        bRightPressed = true;
        if (Velocity.X > Acceleration) Velocity.X = 0;
    }

    if (Velocity.Y > 0)
    {
        Velocity.Y -= Acceleration;
        bDownPressed = true;
        if (Velocity.Y < Acceleration) Velocity.Y = 0;
    }
    else if (Velocity.Y < 0)
    {
        Velocity.Y += Acceleration;
        bUpPressed = true;
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

void APlayerPawn::LoseLife(int32 Amount)
{
    VieCurrent = FMath::Max(0, VieCurrent - FMath::Max(1, Amount));

    if (VieCurrent <= 0)
    {
        // Gérer la mort du joueur (par exemple, recharger le niveau ou afficher un écran de fin de jeu)
        GoToMenu();
    }
}

void APlayerPawn::GoToMenu()
{
    if (MenuLevelName.IsNone())
        return;

    // Charge le niveau menu
    UGameplayStatics::OpenLevel(this, MenuLevelName);
}