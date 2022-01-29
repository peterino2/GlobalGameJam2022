// CREATED FOR A GAMEJAM

#pragma once

#include "Modules/ModuleManager.h"

class FCardGameCardsModule : public FDefaultGameModuleImpl
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
