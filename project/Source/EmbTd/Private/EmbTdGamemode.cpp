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

#include "EmbTdGameMode.h"
#include "EmbTdControllerPB.h"
#include "UObject/ConstructorHelpers.h"

AEmbTdTestGameMode::AEmbTdTestGameMode()
{
	PlayerControllerClass = AEmbTdControllerPB::StaticClass();
}

void AEmbTdTestGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

