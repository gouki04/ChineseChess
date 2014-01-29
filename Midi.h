//
//	Midi
//
//		Copyright (c) 2000 Chihiro.SAKAMOTO (HyperWorks)
//
#ifndef __MIDI_H__
#define __MIDI_H__

class CMidi {
  public:
	CMidi(): Wnd(0), Id(0) {}
	~CMidi() { Close(); }

	BOOL	Open(CWnd *wnd);
	BOOL	Close();

	BOOL	Play(const char *name);
	BOOL	Replay();
	BOOL	Stop();
	BOOL	MciNotify(DWORD id);

  protected:
	CWnd   *Wnd;
	DWORD	Id;
} ;

#endif
