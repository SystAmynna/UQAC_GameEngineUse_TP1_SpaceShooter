// SpaceHUD.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpaceHUD.generated.h"

UCLASS()
class TP1_SPACESHOOTER_API ASpaceHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void DrawHUD() override;
};
