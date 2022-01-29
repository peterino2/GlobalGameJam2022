/** *
 * Copyright (C) 2020-2021
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
#include "Modules/ModuleInterface.h"

class FEmbNav: public FDefaultGameModuleImpl
{
    public:
        static inline FEmbNav& Get()
        {
            return FModuleManager::LoadModuleChecked<FEmbNav>("EmbNav");
        }

        static inline bool IsAvailable()
        {
            return FModuleManager::Get().IsModuleLoaded("EmbNav");
        }

        virtual void StartupModule() override;
        virtual void ShutdownModule() override;
};
