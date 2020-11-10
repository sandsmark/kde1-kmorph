#ifndef __KALPHA__
#define __KALPHA__

#include "morph.h"

#include <ktopwidget.h>
#include <ktoolbar.h>
#include <qpainter.h>
#include <qbutton.h>
#include <kslider.h>
#include <qdialog.h>
#include <qcolor.h>

enum {
		EM_NONE,
		EM_ADD,
		EM_SELECT,
		EM_MOVE,
};

class kalpha : public QWidget
{
	Q_OBJECT
public:
	kalpha(QWidget *parent=0,char *name=0,CMorph *m=0);
	~kalpha();

	void	resizeEvent(QResizeEvent*);
	void	paintEvent(QPaintEvent*);
	void	mousePressEvent(QMouseEvent*);
	void	mouseReleaseEvent(QMouseEvent*);
	void	mouseMoveEvent(QMouseEvent*);
	void	setEditMode(int edit);
	void	setColorMode(bool);
	void	deleteSelected();

public slots:
	// Slider
	void	slide(int);

public:
	void	drawWidgets();
	void	drawAlpha();
	void	drawPoints();
	void	selectLine(int x,int y);
	void	selectNextLine();
	void	selectPrevLine();
	void	selectNoLine();
	void	timerEvent(QTimerEvent *te);

private:

	KSlider		*sl;
	QPainter	*p;
	QColor		selectedcolor,normalcolor;

	CMorph		*morph;

	int		colormode;
	int		rv,gv,bv;
	int		percent;
	int		selected;
	int		editmode;
	int		mx,my;

	int		drawline;

	void	createWidgets();
	void	layoutWidgets();
	void	drawPixel(int,int,int,int,int);
};


#endif


