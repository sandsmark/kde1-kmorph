#ifndef __KMORPHP__
#define __KMORPHP__

#include "morph.h"
#include "kalpha.h"

#include <ktopwidget.h>
#include <ktoolbar.h>
#include <qbutton.h>
#include <qdialog.h>
#include <qpushbt.h>

enum {
		BT_CLOSE = 1,
		BT_MODEADD,
		BT_MODESELECT,
		BT_MODEMOVE,
		BT_MODEMOVESRC,
		BT_MODEMOVEDEST,
		BT_DELETE,
		BT_ZOOMIN,
		BT_ZOOMOUT,
		BT_DISPLAYCOLOR,
		BT_PREV,
		BT_NEXT,
};

class kmorphwin : public KTopLevelWidget
{
	Q_OBJECT
public:
	kmorphwin(QWidget *parent=0,char *name=0,CMorph*m=0);
	~kmorphwin();

	void	resizeEvent(QResizeEvent*);
	void	paintEvent(QPaintEvent*);

protected slots:
	void	updateRects();

public slots:
	// Togglebutton
	void	accept();

	void	modeSelect(bool);
	void	modeAdd(bool);
	void	modeMove(bool);
	void	modeMovePicS(bool);
	void	modeMovePicD(bool);

	void	prevClicked();
	void	nextClicked();

	void	zoominClicked();
	void	zoomoutClicked();

	void	deleteClicked();

	void	modeDisplayColor(bool);

private:
	KToolBar	*tb;

	kalpha		*a;
	CMorph		*morph;

	int		zoom;
	int		movePicS,movePicD;

	void	createWidgets();
};

#endif


