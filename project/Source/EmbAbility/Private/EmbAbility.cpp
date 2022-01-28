// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmbAbility.h"
#include "Modules/ModuleManager.h"

#include "Log.h"

/******************************************************************************
* Function:         virtual void FEmbAbility::StartupModule
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbAbility::StartupModule(){
    UE_LOG(LogEmbAbility, Log, TEXT("EmbAbility module starting up"));
}

/******************************************************************************
* Function:         
* Description:      
* Where:
* Return:           
* Error:            
*****************************************************************************/
void FEmbAbility::ShutdownModule(){
    UE_LOG(LogEmbAbility, Log, TEXT("EmbAbility module shutting down"));
}

 
IMPLEMENT_PRIMARY_GAME_MODULE(FEmbAbility, EmbAbility, "EmbAbility" );
