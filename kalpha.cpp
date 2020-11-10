#include "kalpha.h"
#include "kalpha.moc"

kalpha::kalpha(QWidget *parent=0,char *name=0,CMorph *m=0)
	: QWidget(parent,name)
{
	morph = m;
	colormode = FALSE;
	rv = gv = bv = 1;
	selected = -1;
	editmode = 0;

	selectedcolor.setRgb(255,0,0);
	normalcolor.setRgb(0,255,0);

	createWidgets();
}

kalpha::~kalpha()
{
	delete p;
	delete sl;
}

void	kalpha::createWidgets()
{
	p = new QPainter(this);
	sl = new KSlider(KSlider::Horizontal,this);

	sl->setRange(0,100);
	sl->setValue(50);
//	connect(sl,SIGNAL(sliderMoved(int)),SLOT(slide(int)));
	connect(sl,SIGNAL(valueChanged(int)),SLOT(slide(int)));
}

void	kalpha::layoutWidgets()
{
	int	w,h;

	w = width();
	h = height();

	sl->resize(w,20);
	sl->move(0,h - 20);
	p->setClipRect(0,0,w,h - 21);
}

void	kalpha::resizeEvent(QResizeEvent *re)
{
	layoutWidgets();
}

void	kalpha::paintEvent(QPaintEvent *pe)
{
	//QWidget::paintEvent(pe);
	//drawWidgets();
}

void	kalpha::mousePressEvent(QMouseEvent *me)
{
	QColor		color;

	mx = me->x();
	my = me->y();
	
	if(editmode == EM_ADD)
	{
		color = normalcolor;
		p->setPen(color);

		p->drawRect(mx - 5,my - 5,10,10);
	}
}

void	kalpha::mouseReleaseEvent(QMouseEvent *me)
{
	int		xa,ya,xb,yb;
	int		ww,wh,spw,sph,dpw,dph;
	CPicture	*S,*D;

	S = morph->getSrcPicture();
	D = morph->getDestPicture();

	ww = width();
	wh = height();
	spw = S->getWidth();
	sph = S->getHeight();
	dpw = D->getWidth();
	dph = D->getHeight();

	xa = (mx * spw) / ww;
	ya = (my * sph) / wh;
	xb = (me->x() * dpw) / ww;
	yb = (me->y() * dph) / wh;

	switch(editmode)
	{
		case EM_SELECT:
			selectLine(xb,yb);
			drawPoints();
		break;
		case EM_ADD:
			morph->addPoint(xa,ya,xb,yb);
		break;
		case EM_MOVE:
		break;
	}

	drawPoints();
}

void	kalpha::mouseMoveEvent(QMouseEvent *me)
{
}

void	kalpha::drawWidgets()
{
	if(percent < 0 || percent > 100)
	{
		//sl->setValue(50);
		percent = 50;
	}
	drawAlpha();
	drawPoints();
}

void	kalpha::drawPoints()
{
	int		t,xa,ya,xb,yb;
	int		ww,wh,spw,sph,dpw,dph;
	CPicture	*S,*D;
	QColor		color;

	S = morph->getSrcPicture();
	D = morph->getDestPicture();
	for(t = 0;t <morph->getNPoint();t++)
	{
		morph->getPoint(t,&xa,&ya,&xb,&yb);
		if(t == selected)
		{
			color = selectedcolor;
		}
		else
		{
			color = normalcolor;
		}
		p->setPen(color);
		ww = width();
		wh = height();
		spw = S->getWidth();
		sph = S->getHeight();
		dpw = D->getWidth();
		dph = D->getHeight();

		xa = (xa * ww) / spw;
		ya = (ya * wh) / sph;
		xb = (xb * ww) / dpw;
		yb = (yb * wh) / dph;

		p->drawLine(xa,ya,xb,yb);
		p->drawRect(xa - 5,ya - 5,10,10);
		p->drawEllipse(xb - 5,yb - 5,10,10);
	}
}

void	kalpha::timerEvent(QTimerEvent *te)
{
	if(drawline < height())
	{
		int		x,y;
		int		ww,wh,pw,ph,spw,sph,dpw,dph;
		int		sr,sg,sb,dr,dg,db,r,g,b;
		CPicture	*S,*D,*R;

		S = morph->getSrcPicture();
		D = morph->getDestPicture();
		R = morph->getResultPicture();

		ww = width();
		wh = height();
		spw = S->getWidth();
		sph = S->getHeight();
		dpw = D->getWidth();
		dph = D->getHeight();

		y = drawline;
		for(x = 0;x < ww;x++)
		{
			sr = S->getPixelR((x * spw) / ww,(y * sph) / wh);
			sg = S->getPixelG((x * spw) / ww,(y * sph) / wh);
			sb = S->getPixelB((x * spw) / ww,(y * sph) / wh);
			dr = D->getPixelR((x * dpw) / ww,(y * dph) / wh);
			dg = D->getPixelG((x * dpw) / ww,(y * dph) / wh);
			db = D->getPixelB((x * dpw) / ww,(y * dph) / wh);
/*
			sr = S->getPixelR(x,y);
			sg = S->getPixelG(x,y);
			sb = S->getPixelB(x,y);
			dr = D->getPixelR(x,y);
			dg = D->getPixelG(x,y);
			db = D->getPixelB(x,y);
*/

			r = (sr * (100 - percent) + dr * percent) / 100;
			g = (sg * (100 - percent) + dg * percent) / 100;
			b = (sb * (100 - percent) + db * percent) / 100;
			r = min(255,max(0,r));
			g = min(255,max(0,g));
			b = min(255,max(0,b));
			drawPixel(x,y,r,g,b);
		}
		drawline++;
	}
	else
	{
		killTimers();
		drawPoints();
	}
}

void	kalpha::drawAlpha()
{
	drawline = 0;
	startTimer(30);
}

void	kalpha::drawPixel(int x,int y,int r,int g,int b)
{
	QColor	color;

	if(colormode)
	{
		color.setRgb(r,g,b);
		//printf("Color %i %i %i\n",r,g,b);
	}
	else
	{
		int	v;

		v = (r * rv + g * gv + b * bv) / (rv + gv + bv);
		color.setRgb(v,v,v);
		//printf("Color grey %i rgb %i %i %i push %i %i %i\n",v,r,g,b,rv,gv,bv);
	}

	//p->begin(this);
	p->setPen(color);
	p->drawPoint(x,y);
	//p->end();
}

void	kalpha::slide(int v)
{
	percent = v;
	drawWidgets();
}

void	kalpha::setEditMode(int edit)
{
	editmode = edit;
}

void	kalpha::setColorMode(bool m)
{
	colormode = m;
	drawWidgets();
}

void	kalpha::deleteSelected()
{
	if(selected >= 0) morph->delPoint(selected);
	drawWidgets();
}

void	kalpha::selectLine(int x,int y)
{
	int		d,md,p,t;
	int		xa,ya,xb,yb;

	p = -1;
	md = 1000000000;
	for(t = 0;t < morph->getNPoint();t++)
	{
		morph->getPoint(t,&xa,&ya,&xb,&yb);
		d = (x - xa) * (x - xa) + (y - ya) * (y - ya);
		if(d < md)
		{
			p = t;
			md = d;
		}
		d = sqr(x - xb) + sqr(y - yb);
		if(d < md)
		{
			p = t;
			md = d;
		}
	}
	selected = p;
}

void	kalpha::selectNextLine()
{
	selected++;
	if(selected >= morph->getNPoint()) selected = 0;
	drawPoints();
}

void	kalpha::selectPrevLine()
{
	selected--;
	if(selected < 0) selected = morph->getNPoint() - 1;
	drawPoints();
}

void	kalpha::selectNoLine()
{
	selected = -1;
	drawPoints();
}



