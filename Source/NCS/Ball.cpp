// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!BallCollision)
	{
		// Use a sphere as a simple collision representation.
		BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		BallCollision->BodyInstance.SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
		BallCollision->SetSimulatePhysics(true);
		// Set the sphere's collision radius.
		BallCollision->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = BallCollision;
	}
	if (!BallMovement)
	{
		// Use this component to drive this projectile's movement.
		BallMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		BallMovement->SetUpdatedComponent(BallCollision);
		BallMovement->InitialSpeed = 3000.0f;
		BallMovement->MaxSpeed = 3000.0f;
		BallMovement->bRotationFollowsVelocity = true;
		BallMovement->bShouldBounce = true;
		BallMovement->Bounciness = 0.5f;
		BallMovement->ProjectileGravityScale = 1.0f;
	}

	if (!BallMesh)
	{
		BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			BallMesh->SetStaticMesh(Mesh.Object);
		}
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		BallMat = UMaterialInstanceDynamic::Create(Material.Object, BallMesh);
	}
	BallMesh->SetMaterial(0, BallMat);
	BallMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	BallMesh->SetupAttachment(RootComponent);
}

void ABall::LaunchBall(const FVector& LaunchDirection)
{
	BallMovement->Velocity = LaunchDirection * BallMovement->InitialSpeed;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

