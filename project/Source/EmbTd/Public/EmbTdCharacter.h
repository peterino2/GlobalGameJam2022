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
#include "GameFramework/Character.h"
#include "EmbTdCharacter.generated.h"


UCLASS()
class EMBTD_API AEmbTdCharacter : public ACharacter{
    GENERATED_BODY()

public:
    AEmbTdCharacter();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
};
