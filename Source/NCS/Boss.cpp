// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	check(CharMesh != nullptr);
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	check(BallMesh != nullptr);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetWorld() || IsPlayerVisible())
	{
		AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (!PlayerPawn || !PlayerPawn->IsA(AFPSCharacter::StaticClass())) return;
		AFPSCharacter* ch = Cast<AFPSCharacter>(PlayerPawn);
		FVector Direction = ch->GetActorLocation() - GetActorLocation();
		Direction.Normalize();

		Firre(Direction);
	}
}

bool ABoss::IsPlayerVisible()
{
	FHitResult HitResult;
	FVector StartLocation = GetActorLocation();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return false;
	AActor* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn || !PlayerPawn->IsA(AFPSCharacter::StaticClass())) return false;
	AFPSCharacter* ch = Cast<AFPSCharacter>(PlayerPawn);
	FVector EndLocation = ch->GetActorLocation();
	if (FVector::Dist(StartLocation, EndLocation) > 4000.0f) return false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
	{
		// If the line trace hits something, and it's not the player, return false
		if (HitResult.GetActor() != nullptr && !HitResult.GetActor()->IsA<AFPSCharacter>())
		{
			return false;
		}
	}

	// If the line trace doesn't hit anything or hits the player, return true
	return true;
}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABoss::GiveBall()
{
	if (HasBall || JustFired || JustDamaged || Health <= 0) return false;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Give wolfie ball!"));

	// Update mesh to show ball in hands

	BallMesh->SetVisibility(true);

	return HasBall = true;
}

bool ABoss::Damage()
{
	if (JustDamaged) return false;
	if (Health <= 0) return true;
	JustDamaged = true;
	Health -= 1;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Wolfie damaged!"));
	if (Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Wolfie dead!"));
		// Update mesh to show dead wolfie
		CharMesh->SetVisibility(false);
		GetMesh()->SetVisibility(false);
		BallMesh->SetVisibility(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Destroy actor
		Destroy();
		return true;
	}
	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ABoss::ResetDamage, 0.5f, false);
	return true;
}

void ABoss::ResetDamage()
{
	JustDamaged = false;
}

void ABoss::Firre(const FVector& FireDirection)
{
	if (!HasBall) return;
	GetMesh()->PlayAnimation(ThrowAnim, false);
	FTimerHandle UnusedHandle;

	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this, FireDirection]() {
		Fire(FireDirection);
		}, 0.5f, false);
	JustFired = true;
	HasBall = false;
}

void ABoss::Fire(const FVector& FireDirection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("YARGGHHH FIRRREEE"));
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation;

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = FRotationMatrix::MakeFromX(FireDirection).Rotator();
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				BallMesh->SetVisibility(false);
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				Projectile->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));

				FTimerHandle UnusedHandle;
				World->GetTimerManager().SetTimer(UnusedHandle, this, &ABoss::ResetFire, 0.1f, false);
			}
		}
	}
}

void ABoss::ResetFire()
{
	JustFired = false;
}