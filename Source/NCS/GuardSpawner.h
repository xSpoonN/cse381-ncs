// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuardSpawner.generated.h"

UCLASS()
class NCS_API AGuardSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuardSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle SpawnTimerHandle;
	// Wolfie models
	UClass* a = nullptr;
	UClass* d = nullptr;
	UClass* g = nullptr;
	UClass* n = nullptr;
	UClass* q = nullptr;

	void SpawnGuard();
};
