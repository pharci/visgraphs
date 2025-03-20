
// visgraphs.h: основной файл заголовка для приложения visgraphs
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CvisgraphsApp:
// Сведения о реализации этого класса: visgraphs.cpp
//

class CvisgraphsApp : public CWinApp
{
public:
	CvisgraphsApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CvisgraphsApp theApp;
