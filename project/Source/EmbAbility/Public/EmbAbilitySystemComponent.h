// Copyright (C) 2021-2021 Peter Li - All Rights Reserved The contents of this file and the Embersong Repository is Proprietary and confidential. Unauthorized copying of this file, or any of the code content within the Embersong source directory via any medium is strictly prohibited.Thank you very much for your co-operation Peter Li <Peterlimail47@gmail.com>

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmbAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMBABILITY_API UEmbAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEmbAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
