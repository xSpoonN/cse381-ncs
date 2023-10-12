// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSHUD.h"


int AFPSHUD::ABossCount = -1;
int AFPSHUD::AGuardCount = -1;
int AFPSHUD::GuardsDestroyed = 0;
void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTexture)
	{
		// Find the center of our canvas.
		FVector2D Center(Canvas->ClipX *  0.5f, Canvas->ClipY *  0.5f);

		// Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		// Draw the crosshair at the centerpoint.
		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}

	// Draw boss counter
	if (ABossCount <= 0) {
		FString ScoreText = "YOU WON !! !! FINAL SCORE: ";
		ScoreText.AppendInt(5 + GuardsDestroyed);
		DrawText(ScoreText, FLinearColor::White, 10, 100, NULL, 7.0f);
		return;
	}
	FString BossCountText = "Bosses Remaining: ";
	BossCountText.AppendInt(ABossCount);
	DrawText(BossCountText, FLinearColor::White, 10, 10, NULL, 4.0f);

	FString GuardCountText = "Guards Remaining: ";
	GuardCountText.AppendInt(AGuardCount);
	DrawText(GuardCountText, FLinearColor::White, 10, 50, NULL, 4.0f);

}


void AFPSHUD::OnBossSpawned() {
	AFPSHUD::ABossCount++;
}
void AFPSHUD::OnBossDestroyed() {
	AFPSHUD::ABossCount--;
}
void AFPSHUD::OnGuardSpawned() {
	AFPSHUD::AGuardCount++;
}
void AFPSHUD::OnGuardDestroyed() {
	AFPSHUD::AGuardCount--;
	AFPSHUD::GuardsDestroyed++;
}