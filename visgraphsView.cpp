#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "visgraphs.h"
#endif

#include "visgraphsDoc.h"
#include "visgraphsView.h"

#include "iFuncSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CvisgraphsView, CView)

BEGIN_MESSAGE_MAP(CvisgraphsView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CvisgraphsView::CvisgraphsView() noexcept {
}

CvisgraphsView::~CvisgraphsView() {
}

BOOL CvisgraphsView::PreCreateWindow(CREATESTRUCT& cs) {
	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CvisgraphsView::AssertValid() const {
	CView::AssertValid();
}

void CvisgraphsView::Dump(CDumpContext& dc) const {
	CView::Dump(dc);
}

CvisgraphsDoc* CvisgraphsView::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CvisgraphsDoc)));
	return (CvisgraphsDoc*)m_pDocument;
}
#endif

void DrawFunc(CDC* pDC, CvisgraphsView* pView, CvisgraphsDoc* pDoc, IFuncSolver& fs, COLORREF color) {
	CRect rect;
	pView->GetClientRect(&rect);
	double width = rect.Width();
	double height = rect.Height();

	double scale = pDoc->scale;
	double offsetX = pDoc->offsetX;
	double offsetY = pDoc->offsetY;

	CPen Pen(PS_SOLID, 2, color);
	CPen* pOldPen = pDC->SelectObject(&Pen);

	double x = width / 20 * scale;
	for (double d = -20; d <= 20; d += 0.1) {
		pDC->MoveTo(
			width / 2 + x * d + offsetX,
			height / 2 - x * fs(d) + offsetY
		);
		pDC->LineTo(
			width / 2 + x * (d + 0.1) + offsetX,
			height / 2 - x * fs(d + 0.1) + offsetY
		);
	}

	pDC->SelectObject(pOldPen);
}

void DrawLines(CDC* pDC, CvisgraphsView* pView, CvisgraphsDoc* pDoc) {
	//окно
	CRect rect;
	pView->GetClientRect(&rect);
	double width = rect.Width();
	double height = rect.Height();

	//данные
	double scale = pDoc->scale;
	double offsetX = pDoc->offsetX;
	double offsetY = pDoc->offsetY;

	//перья разных цветов
	CPen PenGray(PS_SOLID, 1, RGB(19, 19, 19));
	CPen PenMediumGray(PS_SOLID, 1, RGB(140, 140, 140));
	CPen PenLightGray(PS_SOLID, 1, RGB(224, 224, 224));

	CPen* pOldPen = pDC->SelectObject(&PenLightGray);

	CString text;
	pDC->SetTextColor(RGB(25, 25, 25));
	//Сетка OX
	for (double i = -30; i <= 30; i++) {
		double x = (width / 2 + width / 20 * scale * i) + offsetX;
		double y = height / 2 + offsetY;

		if (i != 0) {
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, height);
		}

		text.Format(_T("%g"), i);
		pDC->TextOut(x - 4, y + 5, text);
	}

	//Сетка OY
	for (double i = -30; i <= 30; i++) {
		double x = (width / 2) + offsetX;
		double y = (height / 2 - width / 20 * scale * i) + offsetY;

		if (i != 0) {
			pDC->MoveTo(0, y);
			pDC->LineTo(width, y);
		}

		text.Format(_T("%g"), i);
		pDC->TextOut(x - 20, y - 7, text);
	}

	//Ось OX
	pDC->SelectObject(&PenGray); //основная линия серого цвета
	pDC->MoveTo(0, height / 2 + offsetY);
	pDC->LineTo(width, height / 2 + offsetY);

	pDC->SelectObject(&PenMediumGray); //вспомогательная линия светлосерого для красоты
	pDC->MoveTo(0, height / 2 + offsetY + 1);
	pDC->LineTo(width, height / 2 + offsetY + 1);

	//Ось OY
	pDC->SelectObject(&PenGray); //основная линия серого цвета
	pDC->MoveTo(width / 2 + offsetX, 0);
	pDC->LineTo(width / 2 + offsetX, height);

	pDC->SelectObject(&PenMediumGray); //вспомогательная линия светлосерого для красоты
	pDC->MoveTo(width / 2 + offsetX + 1, 0);
	pDC->LineTo(width / 2 + offsetX + 1, height);

	pDC->SelectObject(pOldPen);
}

void CvisgraphsView::OnDraw(CDC* pDC) {
	CRect rect;
	GetClientRect(&rect);
	auto pDoc = GetDocument();

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	DrawLines(&memDC, this, pDoc);

	LinearFunc lf(1, 1);
	SquareFunc sf(1, 0, 0);
	SinFunc sinf;
	CosFunc cosf;

	DrawFunc(&memDC, this, pDoc, lf, RGB(205, 180, 219));
	DrawFunc(&memDC, this, pDoc, sf, RGB(255, 200, 221));
	DrawFunc(&memDC, this, pDoc, sinf, RGB(255, 175, 204));
	DrawFunc(&memDC, this, pDoc, cosf, RGB(162, 210, 255));

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

void CvisgraphsView::OnLButtonDown(UINT nFlags, CPoint point) {
	auto pDoc = GetDocument();
	pDoc->StartDragX = point.x;
	pDoc->StartDragY = point.y;
	pDoc->startDrug = true;

	CView::OnLButtonDown(nFlags, point);
}

void CvisgraphsView::OnMouseMove(UINT nFlags, CPoint point) {
	auto pDoc = GetDocument();
	if (pDoc->startDrug) {
		pDoc->offsetX += (point.x - pDoc->StartDragX);
		pDoc->offsetY += (point.y - pDoc->StartDragY);

		pDoc->StartDragX = point.x;
		pDoc->StartDragY = point.y;

		Invalidate(FALSE);
	}

	// Отслеживание выхода курсора за окно
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = this->GetSafeHwnd();
	tme.dwHoverTime = 0;
	TrackMouseEvent(&tme);

	CView::OnMouseMove(nFlags, point);
}

void CvisgraphsView::OnLButtonUp(UINT nFlags, CPoint point) {
	auto pDoc = GetDocument();
	pDoc->startDrug = false;
	CView::OnLButtonUp(nFlags, point);
}

void CvisgraphsView::OnMouseLeave() {
	auto pDoc = GetDocument();
	pDoc->startDrug = false;
	CView::OnMouseLeave();
}

BOOL CvisgraphsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	auto pDoc = GetDocument();

	if (zDelta > 0) {
		pDoc->scale += 0.1;
	}
	else {
		if (pDoc->scale > 0.5) pDoc->scale -= 0.1;
	}
	Invalidate(FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}