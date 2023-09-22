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

