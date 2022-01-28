// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmbCore.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

/******************************************************************************
* Function:         virtual void FEmbCore::StartupModule
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbCore::StartupModule(){
    UE_LOG(LogEmbCore, Log, TEXT("EmbCore module starting up"));
}

/******************************************************************************
* Function:         
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbCore::ShutdownModule(){
    UE_LOG(LogEmbCore, Log, TEXT("EmbCore module shutting down"));
}

 
IMPLEMENT_GAME_MODULE(FEmbCore, EmbCore);
