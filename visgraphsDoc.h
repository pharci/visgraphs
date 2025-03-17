
// visgraphsDoc.h: интерфейс класса CvisgraphsDoc 
//


#pragma once


class CvisgraphsDoc : public CDocument
{
protected: // создать только из сериализации
	CvisgraphsDoc() noexcept;
	DECLARE_DYNCREATE(CvisgraphsDoc)

// Атрибуты
public:
	double StartDragX = 0.0;
	double StartDragY = 0.0;
	double offsetX = 0.0;
	double offsetY = 0.0;
	bool startDrug = false;
	double scale = 1.0;

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CvisgraphsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
