#include "morph.h"

CPicture::CPicture()
{
	data = NULL;
	xoffset = 0;
	yoffset = 0;
	width = 0;
	height = 0;
}

CPicture::~CPicture()
{
	if(data) free(data);
	data = NULL;
}

void	CPicture::setPicture(int w,int h,byte *pd)
{
	if(!pd) return;

	width = w;
	height = h;
	if(data) free(data);
	data = (byte*)malloc(w * h * 3);
	if(data && pd) memcpy(data,pd,w * h * 3);
}

void	CPicture::setXOffset(int x)
{
	xoffset = x;
}

void	CPicture::setYOffset(int y)
{
	yoffset = y;
}

int	CPicture::getWidth()
{
	return width;
}

int	CPicture::getHeight()
{
	return height;
}

int	CPicture::getXOffset()
{
	return xoffset;
}

int	CPicture::getYOffset()
{
	return yoffset;
}

byte	*CPicture::getPicture()
{
	return data;
}

void	CPicture::setFrameReturnMode(int m)
{
	framereturnmode = m;
}

void	CPicture::setFrameReturn(int fr)
{
	framereturn = fr;
}

void	CPicture::setFrameReturn(byte r,byte g,byte b)
{
	((byte*)&framereturn)[0] = r;
	((byte*)&framereturn)[1] = g;
	((byte*)&framereturn)[2] = b;
}

int	CPicture::getPixelR(int x,int y)
{
	if(x < xoffset || y < yoffset || x >= xoffset + width || y >= yoffset + height)
	{
		if(framereturnmode == MODECOLOR)
		{
			return ((byte*)&framereturn)[0];
		}
		if(framereturnmode == MODEVALUE)
		{
			return framereturn;
		}
		return 0;
	}
	return *(data + (x - xoffset + (y - yoffset) * width) * 3);
}

int	CPicture::getPixelG(int x,int y)
{
	if(x < xoffset || y < yoffset || x >= xoffset + width || y >= yoffset + height)
	{
		if(framereturnmode == MODECOLOR)
		{
			return ((byte*)&framereturn)[1];
		}
		if(framereturnmode == MODEVALUE)
		{
			return framereturn;
		}
		return 0;
	}
	return *(data + (x - xoffset + (y - yoffset) * width) * 3 + 1);
}

int	CPicture::getPixelB(int x,int y)
{
	if(x < xoffset || y < yoffset || x >= xoffset + width || y >= yoffset + height)
	{
		if(framereturnmode == MODECOLOR)
		{
			return ((byte*)&framereturn)[2];
		}
		if(framereturnmode == MODEVALUE)
		{
			return framereturn;
		}
		return 0;
	}
	return *(data + (x - xoffset + (y - yoffset) * width) * 3 + 2);
}

void	CPicture::setPixel(int x,int y,byte r,byte g,byte b)
{
	if(x < xoffset || y < yoffset || x >= xoffset + width || y >= yoffset + height)
	{
		printf("Don't set Pixel\n");

		return;
	}
	*(data + (x - xoffset + (y - yoffset) * width) * 3) = r;
	*(data + (x - xoffset + (y - yoffset) * width) * 3 + 1) = g;
	*(data + (x - xoffset + (y - yoffset) * width) * 3 + 2) = b;
}






CMorph::CMorph()
{
	width = 0;
	height = 0;
	S = D = R = NULL;
	S = new CPicture();
	D = new CPicture();
	R = new CPicture();
	progress = NULL;
	percent = NULL;
	mpoint.resize(0);
	warponly = 0;
}

CMorph::~CMorph()
{
	int	t;

	width = 0;
	height = 0;

	progress = NULL;
	percent = NULL;
	mpoint.resize(0);

	delete S;
	delete D;
	delete R;
}

void	CMorph::setSrcPicture(byte *p,int w,int h)
{
	S->setPicture(w,h,p);
}

void	CMorph::setDestPicture(byte *p,int w,int h)
{
	D->setPicture(w,h,p);
}

void	CMorph::setResultPicture(byte *p,int w,int h)
{
	R->setPicture(w,h,p);
}

CPicture	*CMorph::getSrcPicture()
{
	return S;
}

CPicture	*CMorph::getDestPicture()
{
	return D;
}

CPicture	*CMorph::getResultPicture()
{
	return R;
}

void	CMorph::setFrameSize(int w,int h)
{
	width = w;
	height = h;
}

int	CMorph::getFrameWidth()
{
	return width;
}

int	CMorph::getFrameHeight()
{
	return height;
}

void	CMorph::setWarpOnly(int warp)
{
	warponly = warp;
}

void	CMorph::setMorphIndex(int s,int e)
{
	msi = s;
	mei = e;
}

void	CMorph::setColormorphIndex(int s,int e)
{
	csi = s;
	cei = e;
}

void	CMorph::setSignalProgress(void (*pr)(int,int,int))
{
	progress = pr;
}

void	CMorph::setSignalPercent(void (*p)(int))
{
	percent = p;
}

void	CMorph::addPoint(int xa,int ya,int xb,int yb)
{
	mpoint.resize(mpoint.size() + 1);
	mpoint[mpoint.size() - 1].sx = xa;
	mpoint[mpoint.size() - 1].sy = ya;
	mpoint[mpoint.size() - 1].dx = xb;
	mpoint[mpoint.size() - 1].dy = yb;
}

void	CMorph::delPoint(int n)
{
	int	t;

	for(t = n;t < mpoint.size() - 1;t++)
	{
		mpoint[t] = mpoint[t + 1];
	}
	mpoint.resize(mpoint.size() - 1);
}

void	CMorph::getPoint(int n,int *xa,int *ya,int *xb,int *yb)
{
	*xa = mpoint[n].sx;
	*ya = mpoint[n].sy;
	*xb = mpoint[n].dx;
	*yb = mpoint[n].dy;
}

int	CMorph::getNPoint()
{
	return mpoint.size();
}

void	CMorph::renderFrameLine()
{
	int		x,y,i;
	double		xs,ys,xd,yd,scol,dcol,scp,dcp,rad,sqrd;
	int		xsi,ysi,xdi,ydi,rs,gs,bs,rd,gd,bd,rn,gn,bn;
	char		output[30];
	int		w,h;
	int	 	npoint;

	if(frame > e - s || !rendering)
	{
		rendering = FALSE;
		return;
	}

	frameready = FALSE;

	w = width;
	h = height;

	w = R->getWidth();
	h = R->getHeight();

	npoint = mpoint.size();

	betwx.resize(npoint);
	betwy.resize(npoint);
	srcx.resize(npoint);
	srcy.resize(npoint);
	destx.resize(npoint);
	desty.resize(npoint);
	sqrr.resize(npoint);
	alfxs.resize(npoint);
	alfys.resize(npoint);
	alfxd.resize(npoint);
	alfyd.resize(npoint);

	for(i = 0;i < npoint;i++)
	{
		srcx[i] = mpoint[i].sx;
		srcy[i] = mpoint[i].sy;
		destx[i] = mpoint[i].dx;
		desty[i] = mpoint[i].dy;
	}

	if((i = mei - msi + 2) == 0) return;
	dcp = (double)(frame - msi + 1) / (double)i;
	if((i = cei - csi + 2) == 0) return;
	dcol = (double)(frame - csi + 1) / (double)i;
	if(dcp < 0) dcp = 0;
	if(dcp > 1) dcp = 1;
	if(dcol < 0) dcol = 0;
	if(dcol > 1) dcol = 1;
	scp = 1 - dcp;
	scol = 1 - dcol;

	//printf("%f %f %f %f\n",scp,dcp,scol,dcol);

	calc(scp,dcp,scol,dcol,warponly);

	y = frameline;

	if(!warponly)
	{
		for(x = 0;x < w;x++)
		{
			xd = xs = (double)x;
			yd = ys = (double)y;
			for(i = 0;i < npoint;i++)
			{
				sqrd = sqr((double)betwx[i] - (double)x) + 
				       sqr((double)betwy[i] - (double)y);
				rad = 1.0 / (sqrd + sqrr[i]);
				xs += alfxs[i] * rad;
				ys += alfys[i] * rad;
				xd += alfxd[i] * rad;
				yd += alfyd[i] * rad;
				//printf("%f %f  %f %f  %f\n",xs,ys,xd,yd,rad);
			}
			if((xsi = (int)xs) < 0) xsi = 0;
			if((ysi = (int)ys) < 0) ysi = 0;
			if(xsi >= S->getWidth()) xsi = S->getWidth() - 1;
			if(ysi >= S->getHeight()) ysi = S->getHeight() - 1;
			if((xdi = (int)xd) < 0) xdi = 0;
			if((ydi = (int)yd) < 0) ydi = 0;
			if(xdi >= D->getWidth()) xdi = D->getWidth() - 1;
			if(ydi >= D->getHeight()) ydi = D->getHeight() - 1;
			//printf("%i %i  %i %i -> %i %i\n",xsi,ysi,xdi,ydi,x,y);
			rs = S->getPixelR(xsi,ysi);
			gs = S->getPixelG(xsi,ysi);
			bs = S->getPixelB(xsi,ysi);
			rd = D->getPixelR(xdi,ydi);
			gd = D->getPixelG(xdi,ydi);
			bd = D->getPixelB(xdi,ydi);
			//rn = (int)min(255,max(rs * scol + rd * dcol,0));
			//gn = (int)min(255,max(gs * scol + gd * dcol,0));
			//bn = (int)min(255,max(bs * scol + bd * dcol,0));
			rn = (int)(rs * scol + rd * dcol);
			gn = (int)(gs * scol + gd * dcol);
			bn = (int)(bs * scol + bd * dcol);
			R->setPixel(x,y,rn,gn,bn);
		}
	}
	else
	{
		for(x = 0;x < w;x++)
		{
			xs = x;
			ys = y;
			
			for(i = 0;i < npoint;i++)
			{
				//printf("%f %f  %f %f\n",x,y,betwx[i],betwy[i]);
				sqrd = sqr((double)betwx[i] - (double)x) + 
				       sqr((double)betwy[i] - (double)y);
				rad = 1 / (sqrd + sqrr[i]);
				//printf("%f %f\n",sqrd,sqrr[i]);
				xs += alfxs[i] * rad;
				ys += alfys[i] * rad;
				//printf("%f %f  %f\n",xs,ys,rad);
			}
			if((xsi = (int)xs) < 0) xsi = 0;
			if((ysi = (int)ys) < 0) ysi = 0;
			if(xsi >= S->getWidth()) xsi = S->getWidth() - 1;
			if(ysi >= S->getHeight()) ysi = S->getHeight() - 1;
			//if(x != xsi || y != ysi) printf("%i %i -> %i %i\n",xsi,ysi,x,y);
			R->setPixel(x,y,S->getPixelR(xsi,ysi),
					S->getPixelG(xsi,ysi),
					S->getPixelB(xsi,ysi));
		}
	}

	frameline++;
	if(frameline >= R->getHeight() - 1)
	{
		frameline = 0;
		frameready = TRUE;
		frame++;
	}
}

// private

int	CMorph::gauss(int n,QArray<double> f,QArray<double> fe,QArray<double> fg)
{
	int		t,tt,s;
	double		a;
	QArray<double>	ft,fet;

	ft.resize(n * n);
	for(t = 0;t < n * n;t++) ft[t] = f[t];

	fet.resize(n);
	for(t = 0;t < n;t++) fet[t] = fe[t];


	for(t = 0;t < n - 1;t++)
	{
		for(tt = t + 1;tt < n;tt++)
		{
			if(ft[t + tt * n] == 0)
			{
				return FALSE;
			}
			a = - ft[t + t * n] / ft[t + tt * n];
			fet[tt] *= a;
			for(s = t;s < n;s++)
			{
				ft[s + tt * n] *= a;
			}
		}
		for(tt = t + 1;tt < n;tt++)
		{
			fet[tt] += fet[t];
			for(s = t;s < n;s++)
			{
				ft[s + tt * n] += ft[s + t * n];
			}
		}
	}
	for(t = n - 1;t >= 0;t--)
	{
		a = fet[t];
		for(tt = t + 1;tt < n;tt++)
		{
			a -= fg[tt] * ft[tt + t * n];
		}
		if(ft[t + t * n] == 0)
		{
			return FALSE;
		}
		fg[t] =  a / ft[t + t * n];
	}
	return TRUE;
}



double	CMorph::calc_r(double x,double y,int i)
{
	double		r,min;
	int		j;

	min = 1e20;
	for(j = 0;j < mpoint.size();j++)
	{
		if(j != i)
		{
			r = sqr(x - betwx[j]) + sqr(y - betwy[j]);
			if(r < min) min = r;
		}
	}
	if(min < 10) return 10;
	return min;
}

void	CMorph::calc(double scp,double dcp,double scol,double dcol,int onlywarp)
{
	int		i,j;
	QArray<double>	a,farrx,farry,alfarr;
	int		w,h,npoint;

	w = width;
	h = height;

	npoint = mpoint.size();

	a.resize(npoint * npoint);
	farrx.resize(npoint);
	farry.resize(npoint);
	alfarr.resize(npoint);

	for(i = 0;i < npoint;i++)
	{
		betwx[i] =  (int)(scp * srcx[i] +  dcp * destx[i]);
		betwy[i] =  (int)(scp * srcy[i] +  dcp * desty[i]);
	}
	if(npoint == 1)
	{
		sqrr[0] = (double)sqr(w);
	}
	else
	{
		for(i = 0;i < npoint;i++)
		{
			sqrr[i] = calc_r(betwx[i],betwy[i],i);
		}
	}
	for(i = 0;i < npoint;i++)
	{
		farrx[i] = (double)srcx[i] - (double)betwx[i];
		farry[i] = (double)srcy[i] - (double)betwy[i];
		for(j = 0;j < npoint;j++)
		{
			a[i * npoint + j] = 
				1 / (sqrr[i] + sqr((double)betwx[i] - 
				(double)betwx[j]) + sqr((double)betwy[i] - 
				(double)betwy[j]));
		}
	}
	if(gauss(npoint,a,farrx,alfarr)) for(i =  0;i < npoint;i++) alfxs[i] = alfarr[i];
	if(gauss(npoint,a,farry,alfarr)) for(i =  0;i < npoint;i++) alfys[i] = alfarr[i];
	if(!onlywarp)
	{
		for(i = 0;i < npoint;i++)
		{
			farrx[i] = destx[i] - betwx[i];
			farry[i] = desty[i] - betwy[i];
		}
		if(gauss(npoint,a,farrx,alfarr))
			for(i = 0;i < npoint;i++) alfxd[i] = alfarr[i];
		if(gauss(npoint,a,farry,alfarr))
			for(i = 0;i < npoint;i++) alfyd[i] = alfarr[i];
	}
}

/*
void	CMorph::setupResultPicture()
{
	int	x,y,w,h;
	byte	*pd;

	x = min(S->getXOffset(),D->getXOffset());
	y = min(S->getYOffset(),D->getYOffset());
	w = max(S->getWidth() + S->getXOffset(),D->getWidth() + D->getXOffset());
	h = max(S->getHeight() + S->getYOffset(),D->getHeight() + D->getYOffset());

	w = w - x;
	h = h - y;

	pd = (byte*)malloc(w * h * 3);
	R->setPicture(pd,w,h);
	if(pd) free(pd);
	R->setOffset(x,y);
}
*/
