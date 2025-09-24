// C++
	#include "SpaceProjectile.h"
	#include "Components/StaticMeshComponent.h"
	
	ASpaceProjectile::ASpaceProjectile()
	{
	    PrimaryActorTick.bCanEverTick = true;
	
	    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	    SetRootComponent(MeshComponent);
	
	    // Overlap simple comme les astéroïdes
	    MeshComponent->SetMobility(EComponentMobility::Movable);
	    MeshComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	    MeshComponent->SetGenerateOverlapEvents(true);
	    MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpaceProjectile::OnOverlapBegin);
	
	    InitialLifeSpan = 3.0f; // auto-destruction
	}
	
	void ASpaceProjectile::BeginPlay()
	{
	    Super::BeginPlay();

		GEngine -> AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Pew!"));
	
	    // Ignore son Owner pour éviter l’auto-collision
	    if (AActor* OwnerActor = GetOwner())
	    {
	        MeshComponent->IgnoreActorWhenMoving(OwnerActor, true);
	    }
	}
	
	void ASpaceProjectile::Tick(float DeltaSeconds)
	{
	    Super::Tick(DeltaSeconds);
	
	    // Déplacement à la manière des astéroïdes
	    AddActorWorldOffset(Velocity * DeltaSeconds, /*bSweep=*/false);
	}
	
	void ASpaceProjectile::InitVelocity(const FVector& ShootDir, float Speed)
	{
	    const FVector Dir = ShootDir.GetSafeNormal();
	    Velocity = Dir * Speed;
	    SetActorRotation(Dir.Rotation());
	}
	
	void ASpaceProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                      bool bFromSweep, const FHitResult& SweepResult)
	{
	    if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	        return;
	
	    // Détruire le projectile au premier contact (comportement minimal)
	    Destroy();
	}