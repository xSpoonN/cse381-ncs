// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class NCS_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
    // Primary draw call for the HUD.
    virtual void DrawHUD() override;

    static int ABossCount;
    static int AGuardCount;
    static int GuardsDestroyed;
    static void OnBossSpawned();
    static void OnBossDestroyed();
    static void OnGuardSpawned();
    static void OnGuardDestroyed();

protected:
    // This will be drawn at the center of the screen.
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrosshairTexture;
	

};
