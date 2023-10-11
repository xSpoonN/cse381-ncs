// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Boss.generated.h"

UCLASS()
class NCS_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* CharMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UAnimSequence* ThrowAnim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UObject> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<UObject> BallClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	bool GiveBall(); // Returns true if the player was given a ball
	UFUNCTION()
	bool Damage();

	UFUNCTION()
	void Fire(const FVector& FireDirection);

private:
	bool HasBall = false;
	bool JustFired = false;
	bool JustDamaged = false;
	int8 Health = 3;
	void ResetFire();
	void ResetDamage();
	bool IsPlayerVisible();
	void Firre(const FVector& FireDirection);

	// Movement
	const float Randomness = 1000.0f;
	FVector Destination;
	AAIController* MyController;
};
