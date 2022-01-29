// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FEmbTd: public FDefaultGameModuleImpl
{
    public:
        static inline FEmbTd& Get()
        {
            return FModuleManager::LoadModuleChecked<FEmbTd>("EmbTd");
        }

        static inline bool IsAvailable()
        {
            return FModuleManager::Get().IsModuleLoaded("EmbTd");
        }

        virtual void StartupModule() override;
        virtual void ShutdownModule() override;
};

