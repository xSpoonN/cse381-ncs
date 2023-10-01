// Fill out your copyright notice in the Description page of Project Settings.


#include "BallSpawner.h"
#include "Ball.h"
#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABallSpawner::ABallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABallSpawner::SpawnBall()
{
	TArray<AActor*> FoundBalls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSProjectile::StaticClass(), FoundBalls);
	if (FoundBalls.Num() < MaxBalls)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		AFPSProjectile* SpawnedBall = GetWorld()->SpawnActor<AFPSProjectile>(AFPSProjectile::StaticClass(), SpawnLocation, SpawnRotation);
		UE_LOG(LogTemp, Warning, TEXT("Spawned ball at %s"), *SpawnLocation.ToString());

		if (SpawnedBall)
		{
			SpawnedBall->ProjectileMovementComponent->InitialSpeed = 1000.0f;
		}
	}
}

// Called when the game starts or when spawned
void ABallSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ABallSpawner::SpawnBall, 1.0f, true);
}

// Called every frame
void ABallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

