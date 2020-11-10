#ifndef __KMORPH__
#define __KMORPH__

#include "morph.h"

#include <kapp.h>
#include <kpopmenu.h>
#include <kmenubar.h>
#include <ktopwidget.h>
#include <kstatusbar.h>
#include <qlabel.h>
#include <qlined.h>
#include <qchkbox.h>
#include "KFileDialog.h"
#include "kmorphwin.h"

#define MPopupMenu	QPopupMenu

#define STATSPIC	0x01
#define STATDPIC	0x02
#define STATFMASK	0x04
#define STATMASK	(STATSPIC | STATDPIC | STATFMASK)

class kmorph : public QWidget
{
	Q_OBJECT
public:
	kmorph();

public slots:
	// Menu File
	void	newClicked();
	void	openClicked();
	void	saveClicked();
	void	saveAsClicked();
	void	openPicSClicked();
	void	openPicDClicked();
	void	quitClicked();

	// Menu Edit
	void	editClicked();
	void	renderClicked();

	// Menu Help
	void	helpClicked();
	void	aboutClicked();

	// Checkbutton
	void	warponlyClicked(bool on);

	// Signal
	void	winDestroyed();

	// Button
	void	reloadClicked();

private:
	char		*projectname;
	char		*picsname,*picdname,*picrname,*picrmask;
	int		width,height;
	char		*script;

//	KTopLevelWidget	*tlw;
	KStatusBar	*stbar;
	QWidget		*Container;
	MPopupMenu	*Mfile,*Medit,*Moption,*Mhelp;
	KMenuBar	*mainmenu;
	QLineEdit	*lined[9];
	QLabel		*label[20];
	QCheckBox	*check[1];
	QPushButton	*reload;

	kmorphwin	*mw;

	int		render;
	int		status;

	int		mid[20];

	CMorph		*morph;

	void	createWidgets();
	void	createMenu();
	int	loadPic(char *,byte **,int *,int *);
	int	savePic(char *,byte *,int,int);
	void	timerEvent(QTimerEvent *);
	void	enableMenus();
	void	disableMenusForRender();
	void	disableMenusForEdit();
};

#endif

