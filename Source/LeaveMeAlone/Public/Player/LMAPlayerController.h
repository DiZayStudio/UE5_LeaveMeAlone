// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LMAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALMAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:
	ALMAPlayerController();

	protected:
	virtual void BeginPlay() override;
	virtual void BeginSpectatingState() override;
};
