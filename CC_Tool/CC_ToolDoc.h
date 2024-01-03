// CC_ToolDoc.h : CCC_ToolDoc class interface
//


#pragma once



class CCC_ToolDoc : public CDocument
{
protected: // Create only from serialization.
	CCC_ToolDoc();
	DECLARE_DYNCREATE(CCC_ToolDoc)

	// attribute
public:
	CString		m_strSerial[ MAX_DEVICE];
	CString		m_strSensor[ MAX_DEVICE];
	float		m_dData[MAX_DEVICE][ MAX_DATA][MAX_WAVE];
	float		m_dTemp[MAX_DEVICE][ MAX_DATA];
	int			m_nExp[MAX_DEVICE][ MAX_DATA];
	int			m_nAvg[MAX_DEVICE][ MAX_DATA];
	int			m_nDataNum;
	int			m_nSpectro;
	int			m_nWaveNum;
	int			m_nStartWave;
	CTime		m_dTime[ MAX_DATA];
	int			m_nRefSelect;
	// operation
public:

	// override
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	// implementation
public:
	virtual ~CCC_ToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message allocation function
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DeleteData(int sel);
};


