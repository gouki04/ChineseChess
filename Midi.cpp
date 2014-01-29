//
//	Midi
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#include "stdafx.h"
#include "resource.h"
#include "Midi.h"

// ����Midi

BOOL	CMidi::Open(CWnd *wnd)
{
	Wnd = wnd;
	return TRUE;
}

// �ر�

BOOL	CMidi::Close()
{
	return TRUE;
}

// ����Midi�ļ�

BOOL	CMidi::Play(const char *name)
{
	Stop();

	DWORD	err;
	MCI_OPEN_PARMS	open;
	open.lpstrDeviceType = "sequencer";
	open.lpstrElementName = name;
	if ((err = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE|MCI_OPEN_ELEMENT|MCI_WAIT, (DWORD)&open)) != 0) {
		char	errstr[256];
		mciGetErrorString(err, errstr, sizeof(errstr));
		TRACE1("%s\n", errstr);
		Wnd->MessageBox(errstr);
		return FALSE;
	}
	Id = open.wDeviceID;

	MCI_STATUS_PARMS	status;
	status.dwItem = MCI_SEQ_STATUS_PORT;
	if (mciSendCommand(Id, MCI_STATUS, MCI_STATUS_ITEM|MCI_WAIT, (DWORD)&status)) {
		mciSendCommand(Id, MCI_CLOSE, MCI_WAIT, 0);
		Id = 0;
		return FALSE;
	}
	if (LOWORD(status.dwReturn) != LOWORD(MIDI_MAPPER)) {
		Wnd->MessageBox("MINI��Ӧ���޷�ʹ��");
		mciSendCommand(Id, MCI_CLOSE, MCI_WAIT, 0);
		Id = 0;
		return FALSE;
	}

	MCI_PLAY_PARMS	play;
	play.dwCallback = (DWORD)Wnd->GetSafeHwnd();
	if (mciSendCommand(Id, MCI_PLAY, MCI_NOTIFY, (DWORD)&play)) {
		mciSendCommand(Id, MCI_CLOSE, MCI_WAIT, 0);
		Id = 0;
		return FALSE;
	}
	return TRUE;
}

// ��ͷ���Ŷ�����Midi�ļ�

BOOL	CMidi::Replay()
{
	MCI_PLAY_PARMS	play;

	play.dwCallback = (DWORD)Wnd->GetSafeHwnd();
	if (mciSendCommand(Id, MCI_SEEK, MCI_SEEK_TO_START|MCI_WAIT, 0)
	 || mciSendCommand(Id, MCI_PLAY, MCI_NOTIFY, (DWORD)&play)) {
		mciSendCommand(Id, MCI_CLOSE, MCI_WAIT, 0);
		Id = 0;
	}
	return TRUE;
}

// �������

BOOL	CMidi::Stop()
{
	UINT	id = Id;

	if (Id == 0)
		return FALSE;
	Id = 0;

	DWORD	err;
	if ((err = mciSendCommand(id, MCI_STOP, MCI_WAIT, 0)) != 0
	 || (err = mciSendCommand(id, MCI_CLOSE, MCI_WAIT, 0)) != 0) {
		return FALSE;
	}
	return TRUE;
}

// �������ʱ�����ܴ��ڵ��õĺ���

BOOL	CMidi::MciNotify(DWORD id)
{
	if (Id != id)
		return FALSE;
	return TRUE;
}
