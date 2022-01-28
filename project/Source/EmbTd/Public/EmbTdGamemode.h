/**
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EmbTdGameMode.generated.h"


UCLASS()
class EMBTD_API AEmbTdTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AEmbTdTestGameMode();

protected:
    void Tick(float DeltaSeconds);
};
