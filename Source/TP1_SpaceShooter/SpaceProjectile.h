// C++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceProjectile.generated.h"

UCLASS()
class TP1_SPACESHOOTER_API ASpaceProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASpaceProjectile();

    // Initialise la direction et la vitesse
    void InitVelocity(const FVector& ShootDir, float Speed);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    UStaticMeshComponent* MeshComponent = nullptr;

    // Vitesse courante du projectile (style Asteroid)
    FVector Velocity = FVector::ZeroVector;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);
};