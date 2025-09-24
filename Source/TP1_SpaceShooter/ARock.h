// cpp
	// Fichier: 'Source/TP1_SpaceShooter/ARock.h'
	#pragma once
	
	#include "CoreMinimal.h"
	#include "GameFramework/Actor.h"
	#include "ARock.generated.h"
	
	class UStaticMeshComponent;
	class AARockManager;
	
	UCLASS()
	class TP1_SPACESHOOTER_API AARock : public AActor
	{
		GENERATED_BODY()
	
	public:
		AARock();
	
		virtual void Tick(float DeltaTime) override;
	
		UFUNCTION(BlueprintCallable, Category="Movement")
		void SetVelocity(const FVector& InVelocity);
	
		// Référence au manager pour actions globales
		void SetManager(AARockManager* InManager) { OwnerManager = InManager; }
	
	protected:
		virtual void BeginPlay() override;
	
		// Overlap
		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		                    const FHitResult& SweepResult);
	
	public:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
		UStaticMeshComponent* MeshComponent;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		FVector Velocity = FVector::ZeroVector;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		float RotationSpeed = 50.0f;
	
	private:
		UPROPERTY()
		TWeakObjectPtr<AARockManager> OwnerManager;
	};