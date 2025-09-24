// cpp
// ----- File: 'Source/TP1_SpaceShooter/ARock.h' -----
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARock.generated.h"

class UStaticMeshComponent;

UCLASS()
class TP1_SPACESHOOTER_API AARock : public AActor
{
	GENERATED_BODY()

public:
	AARock();

	virtual void Tick(float DeltaTime) override;

	// Définit la vélocité (direction * vitesse)
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetVelocity(const FVector& InVelocity);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float RotationSpeed = 50.0f;
};