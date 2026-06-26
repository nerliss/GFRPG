// Oleksandr Tkachov 2021-2026


#include "HUD/RPGHUDMain.h"

void ARPGHUDMain::DrawCrosshair(float ScreenWidth, float ScreenHeight)
{
	if (bShowCrosshair && CrosshairTexture)
	{
		DrawTexture(CrosshairTexture, ScreenWidth / 2.f, ScreenHeight / 2.f, 16.f, 16.f, 0.f, 0.f, 1.f, 1.f, FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}
