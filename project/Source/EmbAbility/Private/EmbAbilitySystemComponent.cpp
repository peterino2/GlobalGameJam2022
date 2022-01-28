// Copyright (C) 2021-2021 Peter Li - All Rights Reserved The contents of this file and the Embersong Repository is Proprietary and confidential. Unauthorized copying of this file, or any of the code content within the Embersong source directory via any medium is strictly prohibited.Thank you very much for your co-operation Peter Li <Peterlimail47@gmail.com>


#include "EmbAbilitySystemComponent.h"


// Sets default values for this component's properties
UEmbAbilitySystemComponent::UEmbAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEmbAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEmbAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

