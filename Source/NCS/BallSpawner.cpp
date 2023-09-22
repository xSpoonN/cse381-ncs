// Fill out your copyright notice in the Description page of Project Settings.


#include "BallSpawner.h"
#include "Ball.h"
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundBalls);
	if (FoundBalls.Num() < MaxBalls)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		ABall* SpawnedBall = GetWorld()->SpawnActor<ABall>(ABall::StaticClass(), SpawnLocation, SpawnRotation);
		UE_LOG(LogTemp, Warning, TEXT("Spawned ball at %s"), *SpawnLocation.ToString());
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

