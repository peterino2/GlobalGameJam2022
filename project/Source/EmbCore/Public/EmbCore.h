// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FEmbCore: public FDefaultGameModuleImpl
{
    public:
        static inline FEmbCore& Get()
        {
            return FModuleManager::LoadModuleChecked<FEmbCore>("EmbCore");
        }

        static inline bool IsAvailable()
        {
            return FModuleManager::Get().IsModuleLoaded("EmbCore");
        }

        virtual void StartupModule() override;
        virtual void ShutdownModule() override;
};
