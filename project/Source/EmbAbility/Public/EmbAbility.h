// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FEmbAbility: public FDefaultGameModuleImpl
{
    public:
        static inline FEmbAbility& Get()
        {
            return FModuleManager::LoadModuleChecked<FEmbAbility>("EmbAbility");
        }

        static inline bool IsAvailable()
        {
            return FModuleManager::Get().IsModuleLoaded("EmbAbility");
        }

        virtual void StartupModule() override;
        virtual void ShutdownModule() override;
};
