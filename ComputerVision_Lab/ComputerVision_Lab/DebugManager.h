#pragma once
#include "stdafx.h"
#include <string>
using std::string;

class CComputerVisionLabDlg;

class DebugManager
{
private:
	DebugManager(CEdit*);

	static DebugManager* singleInstance;
	static CEdit* logArea;
public:
	static DebugManager* getInstance();
	static void createInstance(CComputerVisionLabDlg*);
	void pushLog(string& log);
	void pushLog(const char* log);
};

