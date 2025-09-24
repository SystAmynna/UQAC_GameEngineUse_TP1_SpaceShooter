// cpp
// Fichier: 'Source/TP1_SpaceShooter/ARock.cpp'
#include "ARock.h"
#include "ARockManager.h"
#include "APlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "EngineUtils.h"
#include "SpaceProjectile.h"
#include "Engine/Engine.h"

AARock::AARock()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	// Overlap pour générer des événements de collision
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AARock::OnOverlapBegin);

	// Pas de physique pour un mouvement piloté par code
	MeshComponent->SetSimulatePhysics(false);
}

void AARock::BeginPlay()
{
	Super::BeginPlay();

	const FRotator RandRot(
		FMath::FRandRange(0.f, 360.f),
		FMath::FRandRange(0.f, 360.f),
		FMath::FRandRange(0.f, 360.f));
	MeshComponent->AddLocalRotation(RandRot);

	Vie = FMath::RandRange(1, 3);
}

void AARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Déplacement sans sweep pour ne pas bloquer les overlaps
	AddActorWorldOffset(Velocity * DeltaTime, /*bSweep=*/false);

	MeshComponent->AddLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void AARock::SetVelocity(const FVector& InVelocity)
{
	Velocity = InVelocity;
}

void AARock::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// Rock ↔ Rock : détruire les deux
	if (AARock* OtherRock = Cast<AARock>(OtherActor))
	{
		if (IsValid(OtherRock))
		{
			OtherRock->Destroy();
		}
		Destroy();
		return;
	}

	// Player ↔ Rock : -1 vie et destruction de tous les astéroïdes
	if (APlayerPawn* Player = Cast<APlayerPawn>(OtherActor))
	{
		Player->LoseLife(1);

		if (OwnerManager.IsValid())
		{
			OwnerManager->DestroyAllAsteroids();
		}
		else
		{
			// Fallback si pas de manager référencé
			for (TActorIterator<AARock> It(GetWorld()); It; ++It)
			{
				if (IsValid(*It)) { It->Destroy(); }
			}
		}
	}

	if (ASpaceProjectile * Projectile = Cast<ASpaceProjectile>(OtherActor))
	{
		Hit();
		Projectile->Destroy();
	}
	
}

void AARock::Hit()
{
	Vie--;
	const int32 hval = 10;
	const int32 dval = 50;

	

	APlayerPawn* Player = Cast<APlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Vie <= 0)
	{
		if (Player) Player->AddScore(dval);
		Destroy();
	} else if (Player) Player->AddScore(hval);
}