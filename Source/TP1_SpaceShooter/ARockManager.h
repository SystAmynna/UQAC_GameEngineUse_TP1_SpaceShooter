// cpp
// ----- File: 'Source/TP1_SpaceShooter/ARockManager.h' -----
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARockManager.generated.h"

class AARock;

UCLASS()
class TP1_SPACESHOOTER_API AARockManager : public AActor
{
	GENERATED_BODY()

public:
	AARockManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Classe d'astéroïde à instancier (Blueprint ou C++)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	TSubclassOf<AARock> AsteroidClass;

	// Limites de la carte (coin haut-gauche et bas-droit)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	FVector2D TopLeftBorder = FVector2D(-2900.f, -1570.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	FVector2D BottomRightBorder = FVector2D(2900.f, 1570.f);

	// Distance en dehors de la carte pour le spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	float OutsidePadding = 100.f;

	// Intervalle de spawn aléatoire [min, max] en secondes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	float SpawnIntervalMin = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	float SpawnIntervalMax = 2.0f;

	// Vitesse aléatoire [min, max]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MinAsteroidSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MaxAsteroidSpeed = 300.f;

	// Nombre max d'astéroïdes simultanés
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
	int32 MaxAsteroids = 20;

private:
	float SpawnTimer = 0.f;
	float NextSpawnTime = 1.f;
	TArray<AARock*> Asteroids;

	void ScheduleNextSpawn();
	void SpawnAsteroid();
	FVector GetRandomSpawnLocationOutside() const;
	FVector GetRandomInsidePoint() const;
	void CleanupAsteroids();
};