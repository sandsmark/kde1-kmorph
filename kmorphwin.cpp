#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <kapp.h>

#include "kmorphwin.h"
#include "kmorphwin.moc"

#include <klocale.h>
// I took it from kdehelp
// this is just a hack, 'til someone find a better solution
// extern KLocale locale;
// static KLocale* klocale = &locale;
// #define klocale KLocale::getGlobalLocale()


kmorphwin::kmorphwin(QWidget *parent=0,char *name=0,CMorph *m)
	: KTopLevelWidget(name)
{
	morph = m;
	createWidgets();
}

kmorphwin::~kmorphwin()
{
	delete tb;
	delete a;
}

void	kmorphwin::createWidgets()
{
	int		id;
	QPixmap		pm,&qp = pm;
	QString		pixdir = getenv("KDEDIR");

	pixdir += "/lib/pics/toolbar/";

	tb = new KToolBar(this);

	qp.load(pixdir + "exit.xpm");
	tb->insertButton(qp,BT_CLOSE,
		SIGNAL(clicked()),this,
		SLOT(accept()),TRUE,
		klocale->translate("Close editor"),-1);
	tb->insertSeparator();

	qp.load(pixdir + "select.xpm");
	//qp.load("../icons/select.xpm");
	tb->insertButton(qp,BT_MODESELECT,
		SIGNAL(toggled(bool)),this,
		SLOT(modeSelect(bool)),TRUE,
		klocale->translate("Select vector"),-1);
	tb->setToggle(BT_MODESELECT);

	qp.load(pixdir + "line.xpm");
	tb->insertButton(qp,BT_MODEADD,
		SIGNAL(toggled(bool)),this,
		SLOT(modeAdd(bool)),TRUE,
		klocale->translate("Add vector"),-1);
	tb->setToggle(BT_MODEADD);

/*
	qp.load(pixdir + "movesrc.xpm");
	//qp.load("../icons/movesrc.xpm");
	tb->insertButton(qp,BT_MODEMOVESRC,
		SIGNAL(toggled(bool)),this,
		SLOT(modeMovePicS(bool)),TRUE,"Move Source Picture",-1);
	tb->setToggle(BT_MODEMOVESRC);

	qp.load(pixdir + "movedest.xpm");
	//qp.load("../icons/movedest.xpm");
	tb->insertButton(qp,BT_MODEMOVEDEST,
		SIGNAL(toggled(bool)),this,
		SLOT(modeMovePicD(bool)),TRUE,"Move Destination Picture",-1);
	tb->setToggle(BT_MODEMOVEDEST);
*/

	qp.load(pixdir + "back.xpm");
	tb->insertButton(qp,BT_PREV,
		SIGNAL(clicked()),this,
		SLOT(prevClicked()),TRUE,
		klocale->translate("Select previous vector"),-1);

	qp.load(pixdir + "forward.xpm");
	tb->insertButton(qp,BT_NEXT,
		SIGNAL(clicked()),this,
		SLOT(nextClicked()),TRUE,
		klocale->translate("Select next vector"),-1);
	tb->insertSeparator();

	qp.load(pixdir + "delete.xpm");
	tb->insertButton(qp,BT_DELETE,
		SIGNAL(clicked()),this,
		SLOT(deleteClicked()),TRUE,
		klocale->translate("Delete vector"),-1);

	tb->insertSeparator();
/*
	qp.load(pixdir + "zoomout.xpm");
	tb->insertButton(qp,BT_ZOOMOUT,
		SIGNAL(clicked()),this,
		SLOT(zoomoutClicked()),TRUE,"Zoom out",-1);

	qp.load(pixdir + "zoomin.xpm");
	tb->insertButton(qp,BT_ZOOMIN,
		SIGNAL(clicked()),this,
		SLOT(zoominClicked()),TRUE,"Zoom in",-1);

*/

	qp.load(pixdir + "dispcolor.xpm");
	//qp.load("../icons/dispcolor.xpm");
	tb->insertButton(qp,BT_DISPLAYCOLOR,
		SIGNAL(toggled(bool)),this,
		SLOT(modeDisplayColor(bool)),TRUE,
		klocale->translate("Gray / Color display"),-1);
	tb->setToggle(BT_DISPLAYCOLOR);


	tb->enable(KToolBar::Show);
	tb->setBarPos(KToolBar::Bottom);

	a = new kalpha(this,"",morph);
	//a->move(2,2);
	//a->resize(w - 4,h - 35);

	setView(a);
	addToolBar(tb);
}

void	kmorphwin::resizeEvent(QResizeEvent* re)
{
	KTopLevelWidget::resizeEvent(re);
//	a->drawWidgets();
}

void	kmorphwin::paintEvent(QPaintEvent* pe)
{
	KTopLevelWidget::paintEvent(pe);
	a->drawWidgets();
}

void	kmorphwin::accept()
{
	close();
	//close(TRUE);
	delete this;
}

void	kmorphwin::modeSelect(bool on)
{
	if(on)
	{
		tb->setButton(BT_MODEADD,FALSE);
		tb->setButton(BT_MODEMOVE,FALSE);
		tb->setButton(BT_MODEMOVESRC,FALSE);
		tb->setButton(BT_MODEMOVEDEST,FALSE);
		a->setEditMode(EM_SELECT);
	}
	else
	{
		a->setEditMode(EM_NONE);
	}
}

void	kmorphwin::modeAdd(bool on)
{
	if(on)
	{
		tb->setButton(BT_MODESELECT,FALSE);
		tb->setButton(BT_MODEMOVE,FALSE);
		tb->setButton(BT_MODEMOVESRC,FALSE);
		tb->setButton(BT_MODEMOVEDEST,FALSE);
		a->setEditMode(EM_ADD);
	}
	else
	{
		a->setEditMode(EM_NONE);
	}
}

void	kmorphwin::modeMove(bool on)
{
	if(on)
	{
		tb->setButton(BT_MODESELECT,FALSE);
		tb->setButton(BT_MODEADD,FALSE);
		tb->setButton(BT_MODEMOVESRC,FALSE);
		tb->setButton(BT_MODEMOVEDEST,FALSE);
		a->setEditMode(EM_MOVE);
	}
	else
	{
		a->setEditMode(EM_NONE);
	}
}

void	kmorphwin::modeMovePicS(bool on)
{
	movePicS = on;
	if(on)
	{
		tb->setButton(BT_MODEMOVEDEST,FALSE);
		tb->setButton(BT_MODEADD,FALSE);
		tb->setButton(BT_MODESELECT,FALSE);
		tb->setButton(BT_MODEMOVE,FALSE);
		a->setEditMode(EM_NONE);
	}
}

void	kmorphwin::modeMovePicD(bool on)
{
	movePicD = on;
	if(on)
	{
		tb->setButton(BT_MODEMOVESRC,FALSE);
		tb->setButton(BT_MODEADD,FALSE);
		tb->setButton(BT_MODESELECT,FALSE);
		tb->setButton(BT_MODEMOVE,FALSE);
		a->setEditMode(EM_NONE);
	}
}

void	kmorphwin::deleteClicked()
{
	a->deleteSelected();
}

void	kmorphwin::zoominClicked()
{
	zoom++;
}

void	kmorphwin::zoomoutClicked()
{
	zoom--;
}

void	kmorphwin::modeDisplayColor(bool on)
{
	a->setColorMode(on);
}

void	kmorphwin::prevClicked()
{
	a->selectPrevLine();
}

void	kmorphwin::nextClicked()
{
	a->selectNextLine();
}

void	kmorphwin::updateRects()
{
	KTopLevelWidget::updateRects();
}

