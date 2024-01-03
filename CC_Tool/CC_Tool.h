// CC_Tool.h : CC_Tool application main header file
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH before including this file for 'stdafx.h' Please include"
#endif

#include "resource.h"       // main symbol

#define	ulong	unsigned long
#define	ushort	unsigned short
#define	uchar	unsigned char

#define		MAX_DATA	10000

#define		MAX_PIXEL	288
#define		START_WAVE	340
#define		END_WAVE	850
#define		MAX_WAVE	(END_WAVE - START_WAVE+1)

#define		MAX_LED		16
#define		MAX_DEVICE	10
#define		MAX_COMP	10

typedef	struct	{
	int				year;
	int				month;
	int				date;
	int				hour;
	int				minute;
	int				sec;

} TIME_BUF;


typedef	struct	{
	char			name[ 16];
	char			ver[ 16];
	char			kind[ 16];
	char			serial[ 16];
	double			comp[ 6];
	int				method;
	long			exposure;	// us
	float			temperature;
	int				average;
	int				cycle;		// ms
	float			pixel[ MAX_PIXEL];
	float			dark[ MAX_PIXEL];
	float			gain[ MAX_PIXEL];
	TIME_BUF		time;
	float			led[ 4];
	float			calc[ 4];
	int				pos[ 4];
	char			gps[ 256];
	int				data_kind;
	int				led_control;
	char			reserve[ 172-8];
	ulong			checksum;
} CC_BUF;

#define		BUFFER_SIZE		4096

typedef	struct {
	unsigned char	cmd;
	unsigned char	index;
	unsigned short	length;
	unsigned char	buf[ BUFFER_SIZE];
} USB_CMD;

// CCC_ToolApp:
// See CC_Tool.cpp for the implementation of this class.
//

class CCC_ToolApp : public CWinApp
{
public:
	CCC_ToolApp();

	HANDLE	m_hMutex;


	// override
public:
	virtual BOOL InitInstance();

	// implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCC_ToolApp theApp;
