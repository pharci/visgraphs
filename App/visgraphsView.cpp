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

	double x = 50 * scale;
	for (double d = (-width/2 - offsetX) / (50 * scale); d <= (width/2 - offsetX) / (50 * scale); d += 0.1) {
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


	int step = 1;
	if (scale < 0.7) {
		step = 2;
	}
	else if (scale > 0.7) {
		step = 1;
	}

	//Сетка OX
	for (int i = (-width/2 - offsetX) / (50 * scale); i <= (width/2 - offsetX) / (50 * scale); i++) {
		if (i % step != 0) {
			continue;
		}

		double x = (width / 2 + 50 * scale * i) + offsetX;
		double y = height / 2 + offsetY;

		if (i != 0) {
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, height);
		}

		text.Format(_T("%d"), i);
		if (i != 0) {
			pDC->TextOut(x - 4, y + 5, text);
		}
		else {
			pDC->TextOut(x - 20, y + 5, text);
		}
	}

	//Сетка OY
	for (int i = (-height/2 + offsetY) / (50 * scale); i <= (height/2 + offsetY) / (50 * scale); i++) {
		if (i % step != 0) {
			continue;
		}

		double x = (width / 2) + offsetX;
		double y = (height / 2 - 50 * scale * i) + offsetY;

		if (i != 0) {
			pDC->MoveTo(0, y);
			pDC->LineTo(width, y);
		}

		if (i != 0) {
			text.Format(_T("%d"), i);
			pDC->TextOut(x - 20, y - 7, text);
		}
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
	LinearFunc lf2(1, 5);
	SquareFunc sf(1, 0, 0);
	SinFunc sinf;
	CosFunc cosf;
	AbsFunc abslf(lf2);
	kFunc klf(lf, 2);
	kFunc ksinf(sf, -0.5);

	DrawFunc(&memDC, this, pDoc, lf, RGB(205, 180, 219));
	DrawFunc(&memDC, this, pDoc, sf, RGB(255, 200, 221));
	DrawFunc(&memDC, this, pDoc, sinf, RGB(255, 175, 204));
	DrawFunc(&memDC, this, pDoc, cosf, RGB(162, 210, 255));
	DrawFunc(&memDC, this, pDoc, abslf, RGB(52, 252, 12));
	DrawFunc(&memDC, this, pDoc, klf, RGB(52, 63, 12));
	DrawFunc(&memDC, this, pDoc, ksinf, RGB(96, 212, 178));

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