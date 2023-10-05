// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Boss.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSProjectile::BeginOverlap);
		CollisionComponent->SetSimulatePhysics(true);
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.4f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	}

	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}

void AFPSProjectile::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If the other actor is of type AFPSCharacter
	if (OtherActor->IsA(AFPSCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Player Hit Event!"));
		// Cast the actor to AFPSCharacter
		AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
		if (!MyPawn) return;
		UCharacterMovementComponent* Movement = MyPawn->GetCharacterMovement();

		if (Movement)
		{
			FVector Velocity = Movement->Velocity;
			FVector Impulse = Velocity * 100.0f;

			// Apply impulse to projectile
			OverlappedComponent->AddImpulse(Impulse);
		}
	}
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;
	AActor* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn || !PlayerPawn->IsA(AFPSCharacter::StaticClass())) return;
	AFPSCharacter* PlayerRef = Cast<AFPSCharacter>(PlayerPawn);
	float Distance = FVector::Dist(GetActorLocation(), PlayerRef->GetActorLocation());
	if (Distance < 100.0f)
	{
		// Destroy the projectile
		bool succ = PlayerRef->GiveBall();
		if (succ) Destroy();
	}

	TArray<AActor*> Bosses;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss::StaticClass(), Bosses);

	for (AActor* Boss : Bosses)
	{
		float BDistance = FVector::Dist(GetActorLocation(), Boss->GetActorLocation());
		if (BDistance < 100.0f)
		{
			// Destroy the projectile
			bool succ = Cast<ABoss>(Boss)->GiveBall();
			if (succ) Destroy();
		}
	}

}

