// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallSpawner.generated.h"

UCLASS()
class NCS_API ABallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallSpawner();

	void SpawnBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const int32 MaxBalls = 30;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle SpawnTimerHandle1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
