// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "mathutils.h"
#include "preproc_etos.h"
#include "preproc_repeat.h"

class FEmbUtils: public FDefaultGameModuleImpl
{
    public:
        static inline FEmbUtils& Get()
        {
            return FModuleManager::LoadModuleChecked<FEmbUtils>("EmbUtils");
        }

        static inline bool IsAvailable()
        {
            return FModuleManager::Get().IsModuleLoaded("EmbUtils");
        }

        virtual void StartupModule() override;
        virtual void ShutdownModule() override;
};
