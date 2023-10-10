// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardSpawner.h"
#include "Guard.h"

// Sets default values
AGuardSpawner::AGuardSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> a_finder(TEXT("/Script/Engine.Blueprint'/Game/BP_GuardA.BP_GuardA'"));
	a = (UClass*)a_finder.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> d_finder(TEXT("/Script/Engine.Blueprint'/Game/BP_GuardD.BP_GuardD'"));
	d = (UClass*)d_finder.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> g_finder(TEXT("/Script/Engine.Blueprint'/Game/BP_GuardG.BP_GuardG'"));
	g = (UClass*)g_finder.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> n_finder(TEXT("/Script/Engine.Blueprint'/Game/BP_GuardN.BP_GuardN'"));
	n = (UClass*)n_finder.Object->GeneratedClass;

	static ConstructorHelpers::FObjectFinder<UBlueprint> q_finder(TEXT("/Script/Engine.Blueprint'/Game/BP_GuardQ.BP_GuardQ'"));
	q = (UClass*)q_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AGuardSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGuardSpawner::SpawnGuard, 5.0f, true);
}

// Called every frame
void AGuardSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGuardSpawner::SpawnGuard()
{
	// Get random guard to spawn
	UClass* bp = nullptr;
	switch (FMath::RandRange(0, 4)) {
	case 0:
		bp = a;
		break;
	case 1:
		bp = d;
		break;
	case 2:
		bp = g;
		break;
	case 3:
		bp = n;
		break;
	case 4:
		bp = q;
		break;
	}

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	AGuard* Guard = GetWorld()->SpawnActor<AGuard>(bp, SpawnLocation, SpawnRotation);
	AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), SpawnLocation, SpawnRotation);
	AIController->Possess(Guard);
	UE_LOG(LogTemp, Warning, TEXT("Guard spawned at %s"), *SpawnLocation.ToString());

}

