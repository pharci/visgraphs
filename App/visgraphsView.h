
// visgraphsView.h: интерфейс класса CvisgraphsView
//

#pragma once


class CvisgraphsView : public CView
{
protected: // создать только из сериализации
	CvisgraphsView() noexcept;
	DECLARE_DYNCREATE(CvisgraphsView)

// Атрибуты
public:
	CvisgraphsDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	virtual ~CvisgraphsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseLeave();
};

#ifndef _DEBUG  // версия отладки в visgraphsView.cpp
inline CvisgraphsDoc* CvisgraphsView::GetDocument() const
   { return reinterpret_cast<CvisgraphsDoc*>(m_pDocument); }
#endif

