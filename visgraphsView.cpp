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
END_MESSAGE_MAP()

CvisgraphsView::CvisgraphsView() noexcept {
}

CvisgraphsView::~CvisgraphsView() {
}

BOOL CvisgraphsView::PreCreateWindow(CREATESTRUCT& cs) {
	return CView::PreCreateWindow(cs);
}

void DrawFunc(CDC* pDC, CvisgraphsView* pView, IFuncSolver& fs, double offsetX, double offsetY, double scale) {
	CRect rect;
	pView->GetClientRect(&rect);
	double width = rect.Width();
	double height = rect.Height();
	double nullX = width / 2;
	double nullY = height / 2;

	std::cout << "name: " << fs.getName() << std::endl;

	CPen blackPen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&blackPen);

	pDC->MoveTo(nullX + offsetX + width / 20 * scale * -20, nullY + offsetY - width / 20 * scale * fs(-20));
	for (double d = -20; d <= 20; d += 0.1) {
		double x = nullX + offsetX + width / 20 * scale * d;
		double y = nullY + offsetY - width / 20 * scale * fs(d);

		pDC->LineTo(x, y);
		pDC->MoveTo(x, y);
	}

	pDC->SelectObject(pOldPen);
}

void DrawLines(CDC* pDC, CvisgraphsView* pView, double offsetX, double offsetY, double scale) {
	CRect rect;
	pView->GetClientRect(&rect);

	double width = rect.Width();
	double height = rect.Height();

	CFont font;
	CFont* pOldFont = pDC->SelectObject(&font);

	CPen Pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&Pen);

	//Ось OX
	pDC->MoveTo(-width + offsetX, height / 2 + offsetY);
	pDC->LineTo(2 * width + offsetX, height / 2 + offsetY);

	//Ось OY
	pDC->MoveTo(width / 2 + offsetX, -width + offsetY);
	pDC->LineTo(width / 2 + offsetX, 2 * width + offsetY);

	CString text;
	//Штрихи на OX
	for (double i = -20; i <= 20; i++) {
		double x = (width/2 + width / 20 * scale * i) + offsetX;
		double y = height/2 + offsetY;

		pDC->MoveTo(x, y - 3);
		pDC->LineTo(x, y + 3);

		text.Format(_T("%g"), i);
		pDC->TextOut(x - 4, y + 5, text);
	}

	//Штрихи на OY
	for (double i = -20; i <= 20; i++) {
		double x = (width / 2) + offsetX;
		double y = (height / 2 - width / 20 * scale * i) + offsetY;

		pDC->MoveTo(x - 3, y);
		pDC->LineTo(x + 3, y);

		text.Format(_T("%g"), i);
		pDC->TextOut(x - 20, y - 7, text);
	}

	pDC->SelectObject(pOldPen);

}

void CvisgraphsView::OnDraw(CDC* pDC) {	

	auto pDoc = GetDocument();
	
	DrawLines(pDC, this, pDoc->offsetX, pDoc->offsetY, pDoc->scale);

	LinearFunc lf(1, 1);
	SquareFunc sf(1, 0, 0);
	SinFunc sinf;
	CosFunc cosf;

	DrawFunc(pDC, this, lf, pDoc->offsetX, pDoc->offsetY, pDoc->scale);
	DrawFunc(pDC, this, sf, pDoc->offsetX, pDoc->offsetY, pDoc->scale);
	DrawFunc(pDC, this, sinf, pDoc->offsetX, pDoc->offsetY, pDoc->scale);
	DrawFunc(pDC, this, cosf, pDoc->offsetX, pDoc->offsetY, pDoc->scale);
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

		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

void CvisgraphsView::OnLButtonUp(UINT nFlags, CPoint point) {
	auto pDoc = GetDocument();
	pDoc->startDrug = false;

	CView::OnLButtonUp(nFlags, point);
}

BOOL CvisgraphsView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	auto pDoc = GetDocument();

	if (zDelta > 0) {
		pDoc->scale += 0.1;
	}
	else {
		if (pDoc->scale > 0.5) pDoc->scale -= 0.1;
	}
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
