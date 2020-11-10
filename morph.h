#ifndef __MORPH__
#define __MORPH__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//#include <qstring.h>
#include <qarray.h>

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE !FALSE
#endif

#define sqr(a)		(a) * (a)
#define min(a,b)	((a) > (b) ? (b) : (a))
#define max(a,b)	((a) < (b) ? (b) : (a))

#define MODECOLOR	0
#define MODEVALUE	1
#define MODEINTER	2

typedef struct
{
	int	sx,sy,dx,dy;
}
mp_t;

class CPicture
{
protected:
	int	width,height;
	int	xoffset,yoffset;

	int	framereturnmode;
	int	framereturn;

	byte	*data;

public:
	CPicture();
	~CPicture();

	void	setPicture(int w,int h,byte *picdata);
	void	setXOffset(int x);
	void	setYOffset(int y);

	int	getWidth();
	int	getHeight();
	int	getXOffset();
	int	getYOffset();
	byte	*getPicture();

	void	setFrameReturnMode(int m);
	void	setFrameReturn(int fr);
	void	setFrameReturn(byte r,byte g,byte b);

	int	getPixelR(int x,int y);
	int	getPixelG(int x,int y);
	int	getPixelB(int x,int y);

	void	setPixel(int x,int y,byte r,byte g,byte b);
};

class CMorph
{
protected:
	// Pictures
	CPicture	*S,*D,*R;
	int		width,height;

	// Counter
	int	frame,frameline;
	int	s,e;

	// Render
	int	rendering;
	int	frameready;

	// Morphing
	int	warponly;
	int	msi;			// MorphStartIndex
	int	mei;			// MorphEndIndex
	int	csi;			// ColormorphStartIndex
	int	cei;			// ColormorphEndIndex

	// Signals
	void	(*progress)(int p,int y,int x);
	void	(*percent)(int p);

	// Points
	QArray<mp_t>	mpoint;

	// Temp
	QArray<int>	betwx,betwy;
	QArray<int>	srcx,srcy,destx,desty;
	QArray<double>	sqrr;
	QArray<double>	alfxs,alfys,alfxd,alfyd;

public:
	CMorph();
	~CMorph();

	void	setSrcPicture(byte *p,int w,int h);
	void	setDestPicture(byte *p,int w,int h);
	void	setResultPicture(byte *p,int w,int h);

	CPicture	*getSrcPicture();
	CPicture	*getDestPicture();
	CPicture	*getResultPicture();

	void	setFrameSize(int w,int h);
	int	getFrameWidth();
	int	getFrameHeight();

	void	setWarpOnly(int warp);

	void	setMorphIndex(int s,int e);
	void	setColormorphIndex(int s,int e);

	void	setSignalProgress(void (*pr)(int,int,int));
	void	setSignalPercent(void (*p)(int));

	void	addPoint(int xa,int ya,int xb,int yb);
	void	delPoint(int n);

	void	getPoint(int n,int *xa,int *ya,int *xb,int *yb);
	int	getNPoint();

	void	startRender(int a,int b) {s = a;e = b;frame = s;frameline = 0;frameready = FALSE;rendering = TRUE;};
	void	renderFrameLine();
	int	frameReady() { return frameready;};
	int	isRendering() { return rendering;};
	int	getRenderLine() { return frameline;};
	int	getRenderFrame() { return frame;};
	int	getRenderLines() { return R->getHeight();};
	int	getRenderFrames() { return e - s ;};
	void	stopRendering() { rendering = FALSE;};

private:
	int	gauss(int n,QArray<double> f,QArray<double> fe,QArray<double> fg);
	double	calc_r(double x,double y,int i);
	void	calc(double scp,double dcp,double scol,double dcol,int onlywarp);

};

#endif

