// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"

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

	/* static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/teamg/Plane.Plane'"));
	if (SkeletalMeshAsset.Succeeded())
		Mesh->SetSkeletalMesh(SkeletalMeshAsset.Object);
	*/

	MyController = GetController<AAIController>();
	if (MyController)
		MyController->Possess(this);

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn) {
		MyController->MoveTo(PlayerPawn->GetActorLocation());
	}

}

// Called to bind functionality to input
void AGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

