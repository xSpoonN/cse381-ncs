// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Ball.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	check(BallMesh != nullptr);
	DamageFlashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageFlashMesh"));
	DamageFlashMesh->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	check(DamageFlashMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);
	BallMesh->SetOnlyOwnerSee(true);
	DamageFlashMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	BallMesh->SetupAttachment(FPSCameraComponent);
	DamageFlashMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Huh"));*/

	BallMesh->SetVisibility(false);
}

bool AFPSCharacter::GiveBall()
{
	if (HasBall || JustFired) return false;
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Give ball!"));*/

	// Update mesh to show ball in hands

	BallMesh->SetVisibility(true);

	return HasBall = true;
}
// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	/*PlayerInputComponent->BindAction("Fire2", IE_Pressed, this, &AFPSCharacter::Fire2);*/

}

void AFPSCharacter::Fire()
{
	if (!HasBall) return;
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
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
				JustFired = true;
				HasBall = false;
				BallMesh->SetVisibility(false);
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);

				FTimerHandle UnusedHandle;
				World->GetTimerManager().SetTimer(UnusedHandle, this, &AFPSCharacter::ResetFire, 0.1f, false);
			}
		}
	}
}

void AFPSCharacter::ResetFire()
{
	JustFired = false;
}

void AFPSCharacter::Fire2()
{
	// Attempt to fire a projectile.
	if (BallClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ABall* Projectile = World->SpawnActor<ABall>(BallClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			//if (Projectile)
			//{
			//	// Set the projectile's initial trajectory.
			//	FVector LaunchDirection = MuzzleRotation.Vector();
			//	Projectile->FireInDirection(LaunchDirection);
			//}
		}
	}
}

void AFPSCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Damage()
{
	if (JustDamaged) return;
	JustDamaged = true;

	if (!JustFired) {
		UGameplayStatics::PlaySound2D(this, DamageSound);
		DamageFlashMesh->SetVisibility(true);
		FTimerHandle UnusedHandle1;
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle1, this, &AFPSCharacter::ResetDamageFlash, 0.15f, false);
	}

	FTimerHandle UnusedHandle;
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &AFPSCharacter::ResetDamage, 0.1f, false);
}

void AFPSCharacter::ResetDamage()
{
	JustDamaged = false;
}

void AFPSCharacter::ResetDamageFlash()
{
	DamageFlashMesh->SetVisibility(false);
}