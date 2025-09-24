// ----- File: 'Source/TP1_SpaceShooter/ARock.cpp' -----
#include "ARock.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

AARock::AARock()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	MeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	MeshComponent->SetSimulatePhysics(false);
}

void AARock::BeginPlay()
{
	Super::BeginPlay();

	// Rotation initiale aléatoire
	const FRotator RandRot(
		FMath::FRandRange(0.f, 360.f),
		FMath::FRandRange(0.f, 360.f),
		FMath::FRandRange(0.f, 360.f));
	MeshComponent->AddLocalRotation(RandRot);
}

void AARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Déplacement
	AddActorWorldOffset(Velocity * DeltaTime * 10.0f, true);

	// Rotation
	MeshComponent->AddLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void AARock::SetVelocity(const FVector& InVelocity)
{
	Velocity = InVelocity;
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Yellow, FString::Printf(TEXT("Asteroid Velocity set to: %s"), *Velocity.ToString()) );
}