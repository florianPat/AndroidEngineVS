#pragma once

#include "Types.h"

struct ActivityHandler
{
	virtual ~ActivityHandler() {};

	virtual STATUS onActivate() = 0;
	virtual void onDeactivate() = 0;
	virtual STATUS onStep() = 0;

	virtual void onStart() {};
	virtual void onResume() {};
	virtual void onPause() {};
	virtual void onStop() {};
	virtual void onDestroy() {};

	virtual void onSaveInstanceState(void** pData, size_t* pSize) {};
	virtual void onConfigurationChanged() {};
	virtual void onLowMemory() {};

	virtual void onCreateWindow() {};
	virtual void onDestroyWindow() {};
	virtual void onGainFocus() {};
	virtual void onLoseFocus() {};
};