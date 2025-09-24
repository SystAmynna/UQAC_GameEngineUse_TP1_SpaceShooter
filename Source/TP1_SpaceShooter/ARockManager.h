// cpp
	// Fichier: 'Source/TP1_SpaceShooter/ARockManager.h'
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
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		TSubclassOf<AARock> AsteroidClass;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		FVector2D TopLeftBorder = FVector2D(-2900.f, -1570.f);
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		FVector2D BottomRightBorder = FVector2D(2900.f, 1570.f);
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		float OutsidePadding = 100.f;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		float SpawnIntervalMin = 0.5f;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		float SpawnIntervalMax = 2.0f;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		float MinAsteroidSpeed = 100.f;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		float MaxAsteroidSpeed = 300.f;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn")
		int32 MaxAsteroids = 20;
	
		// Destruction globale
		UFUNCTION(BlueprintCallable, Category="Spawn")
		void DestroyAllAsteroids();
	
	private:
		float SpawnTimer = 0.f;
		float NextSpawnTime = 1.f;
		TArray<AARock*> Asteroids;
		float DefaultZ = 500.0f;
	
		void ScheduleNextSpawn();
		void SpawnAsteroid();
		FVector GetRandomSpawnLocationOutside() const;
		FVector GetRandomInsidePoint() const;
		void CleanupAsteroids();
	};