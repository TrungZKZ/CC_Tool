// CC_ToolDoc.cpp : Implementation of CCC_ToolDoc class
//

#include "stdafx.h"
#include "CC_Tool.h"

#include "CC_ToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCC_ToolDoc

IMPLEMENT_DYNCREATE(CCC_ToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CCC_ToolDoc, CDocument)
END_MESSAGE_MAP()


// CCC_ToolDoc Construction/Destruction

CCC_ToolDoc::CCC_ToolDoc()
{
	int		i;

	for( i = 0 ; i < MAX_DEVICE; i++){
		m_strSerial[ i] = "";
		m_strSensor[ i] = "";
	}
	m_nDataNum		= 0;
	m_nSpectro		= 0;
	m_nWaveNum		= END_WAVE - START_WAVE + 1;
	m_nStartWave	= START_WAVE;
	m_nRefSelect	= -1;


}

CCC_ToolDoc::~CCC_ToolDoc()
{
}

BOOL CCC_ToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Add reinitialization processing at this location.
	// (SDI documentation reuses this document.)

	return TRUE;
}




// CCC_ToolDoc serialization

void CCC_ToolDoc::Serialize(CArchive& ar)
{
	int	i, j, k;

	if (ar.IsStoring())
	{
		ar << m_nDataNum;
		ar << m_nSpectro;
		ar << m_nWaveNum;
		ar << m_nStartWave;
		ar << m_nRefSelect;
		for( i = 0 ; i < m_nSpectro; i++){
			ar << m_strSerial[ i];
			ar << m_strSensor[ i];
			for( j = 0 ; j < m_nDataNum; j++){
				for( k = 0 ; k < m_nWaveNum; k++){
					ar << m_dData[ i][ j][ k];
				}
				ar << m_dTemp[ i][ j];
				ar << m_nExp[ i][ j];
				ar << m_nAvg[ i][ j];
			}
		}
		for( j = 0 ; j < m_nDataNum; j++){
			ar << m_dTime[ j];
		}
	}
	else
	{
		ar >> m_nDataNum;
		ar >> m_nSpectro;
		ar >> m_nWaveNum;
		ar >> m_nStartWave;
		ar >> m_nRefSelect;
		for( i = 0 ; i < m_nSpectro; i++){
			ar >> m_strSerial[ i];
			ar >> m_strSensor[ i];
			for( j = 0 ; j < m_nDataNum; j++){
				for( k = 0 ; k < m_nWaveNum; k++){
					ar >> m_dData[ i][ j][ k];
				}
				ar >> m_dTemp[ i][ j];
				ar >> m_nExp[ i][ j];
				ar >> m_nAvg[ i][ j];
			}
		}
		for( j = 0 ; j < m_nDataNum; j++){
			ar >> m_dTime[ j];
		}
	}
}


// CCC_ToolDoc diagnostics

#ifdef _DEBUG
void CCC_ToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCC_ToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCC_ToolDoc command


void CCC_ToolDoc::DeleteData( int sel)
{
	int	i, j, k;

	if( sel < m_nDataNum){
		if( sel == m_nRefSelect){
			m_nRefSelect = -1;
		}
		for( i = 0 ; i < m_nSpectro; i++){
			for( j = sel ; j < (m_nDataNum-1); j++){
				for( k = 0 ; k < m_nWaveNum; k++){
					m_dData[ i][ j][ k]	= m_dData[ i][ j+1][ k];
				}
				m_dTemp[ i][ j]	= m_dTemp[ i][ j+1];
				m_nExp[ i][ j]	= m_nExp[ i][ j+1];
			}
		}
		for( j = sel ; j < (m_nDataNum-1); j++){
			m_dTime[ j] = m_dTime[ j+1];
		}
		m_nDataNum--;
	}
}
