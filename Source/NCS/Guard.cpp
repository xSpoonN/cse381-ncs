// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "NavigationSystem.h"
#include "FPSCharacter.h"
#include "FPSHUD.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	MyController = GetController<AAIController>();
	if (MyController)
		MyController->Possess(this);
	AFPSHUD::OnGuardSpawned(GetWorld());
	
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Spawned guy!"));
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MyController) {
		MyController = GetController<AAIController>();
		if (MyController)
			MyController->Possess(this);
		else
			return;
	}
	if (MyController->GetMoveStatus() == EPathFollowingStatus::Idle || MyController->GetMoveStatus() == EPathFollowingStatus::Waiting) {
		// Generate new destination

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

		if (NavSystem) {
			FNavLocation ResultLocation;
			if (NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), Randomness, ResultLocation))
				Destination = ResultLocation.Location;
		}
	}

	MyController->MoveTo(Destination);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;
	AActor* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn || !PlayerPawn->IsA(AFPSCharacter::StaticClass())) return;
	AFPSCharacter* PlayerRef = Cast<AFPSCharacter>(PlayerPawn);
	// Get location slightly above capsule
	FVector PlayerAboveLoc = PlayerRef->GetActorLocation() + FVector(0, 0, PlayerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	FVector BallLocation = GetActorLocation();

	if (FVector::Dist(BallLocation, PlayerRef->GetActorLocation()) < 100.0f || FVector::Dist(BallLocation, PlayerAboveLoc) < 100.0f)
	{
		PlayerRef->Damage();
	}

}

// Called to bind functionality to input
void AGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

