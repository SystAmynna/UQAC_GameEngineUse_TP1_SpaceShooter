// ----- File: 'Source/TP1_SpaceShooter/ARockManager.cpp' -----
#include "ARockManager.h"
#include "ARock.h"
#include "Engine/World.h"

AARockManager::AARockManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARockManager::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNextSpawn();
}

void AARockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer += DeltaTime;

	if (SpawnTimer >= NextSpawnTime && Asteroids.Num() < MaxAsteroids)
	{
		SpawnAsteroid();
		SpawnTimer = 0.f;
		ScheduleNextSpawn();
	}

	CleanupAsteroids();
}

void AARockManager::ScheduleNextSpawn()
{
	NextSpawnTime = FMath::FRandRange(SpawnIntervalMin, SpawnIntervalMax);
}

void AARockManager::SpawnAsteroid()
{
	if (!AsteroidClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector SpawnLocation = GetRandomSpawnLocationOutside();
	const FVector TargetLocation = GetRandomInsidePoint();

	const FVector Dir = (TargetLocation - SpawnLocation).GetSafeNormal();
	const float Speed = FMath::FRandRange(MinAsteroidSpeed, MaxAsteroidSpeed);
	const FVector Velocity = FVector(Dir.X * Speed, Dir.Y * Speed, 0.f);

	AARock* NewAsteroid = World->SpawnActor<AARock>(AsteroidClass, SpawnLocation, FRotator::ZeroRotator);
	if (NewAsteroid)
	{
		NewAsteroid->SetVelocity(Velocity);
		Asteroids.Add(NewAsteroid);
	}
}

FVector AARockManager::GetRandomSpawnLocationOutside() const
{
	const int32 Side = FMath::RandRange(0, 3);
	const float LeftX = TopLeftBorder.X;
	const float RightX = BottomRightBorder.X;
	const float TopY = TopLeftBorder.Y;
	const float BottomY = BottomRightBorder.Y;

	float X = 0.f, Y = 0.f;

	switch (Side)
	{
		// Haut
		case 0:
			X = FMath::FRandRange(LeftX, RightX);
			Y = TopY - OutsidePadding;
			break;
		// Droite
		case 1:
			X = RightX + OutsidePadding;
			Y = FMath::FRandRange(TopY, BottomY);
			break;
		// Bas
		case 2:
			X = FMath::FRandRange(LeftX, RightX);
			Y = BottomY + OutsidePadding;
			break;
		// Gauche
		default:
			X = LeftX - OutsidePadding;
			Y = FMath::FRandRange(TopY, BottomY);
			break;
	}

	return FVector(X, Y, 500.f);
}

FVector AARockManager::GetRandomInsidePoint() const
{
	const float X = FMath::FRandRange(TopLeftBorder.X, BottomRightBorder.X);
	const float Y = FMath::FRandRange(TopLeftBorder.Y, BottomRightBorder.Y);
	return FVector(X, Y, 0.f);
}

void AARockManager::CleanupAsteroids()
{
	for (int32 i = Asteroids.Num() - 1; i >= 0; --i)
	{
		AARock* Rock = Asteroids.IsValidIndex(i) ? Asteroids[i] : nullptr;
		if (!IsValid(Rock))
		{
			Asteroids.RemoveAt(i);
			continue;
		}

		const FVector Loc = Rock->GetActorLocation();
		const float Margin = OutsidePadding + 400.f;

		const bool bOutside =
			Loc.X < TopLeftBorder.X - Margin ||
			Loc.X > BottomRightBorder.X + Margin ||
			Loc.Y < TopLeftBorder.Y - Margin ||
			Loc.Y > BottomRightBorder.Y + Margin;

		if (bOutside)
		{
			Rock->Destroy();
			Asteroids.RemoveAt(i);
		}
	}
}