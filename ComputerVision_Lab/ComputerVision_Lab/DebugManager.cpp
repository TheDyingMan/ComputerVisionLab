#include "stdafx.h"
#include "DebugManager.h"

#include "ComputerVision_LabDlg.h"

DebugManager* DebugManager::singleInstance = nullptr;
CEdit* DebugManager::logArea = nullptr;

DebugManager * DebugManager::getInstance()
{
	ASSERT(singleInstance == nullptr);
	return singleInstance;
}

void DebugManager::createInstance(CComputerVisionLabDlg* dlg)
{
	logArea = &(dlg->DebugInfo);
}

void DebugManager::pushLog(string & log)
{
	CString text;
	logArea->GetWindowTextW(text);
	CString logStr = CString(log.c_str());
	//logStr.Format(_T("%s"),log.c_str());
	text.Append(logStr);

	logArea->SetWindowTextW(text);
}

void DebugManager::pushLog(const char * log)
{
	CString text;
	logArea->GetWindowTextW(text);
	CString logStr = CString(log);
	/*logStr.Format(_T("hello",log));*/
	text.Append(logStr);

	logArea->SetWindowTextW(text);
}
