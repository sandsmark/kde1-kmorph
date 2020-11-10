//
// kmorph - KDE
//

#include <glib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <qkeycode.h>
#include <kmsgbox.h>
#include <kintegerline.h>
#include <qframe.h>

#include <klocale.h>

#define stloc(a)	((char*)klocale->translate((a)))

#include "kmorph.h"
#include "kmorph.moc"

KApplication	*mykapp;

int	main(int argc,char **argv)
{
	kmorph	*win;

	mykapp = new KApplication(argc,argv,"kmorph");

	win = new kmorph();

	mykapp->setMainWidget(win);

	win->resize(435,345);
	win->setMaximumSize(435,345);
	win->setMinimumSize(435,345);

	win->show();

	return mykapp->exec();
}

kmorph::kmorph()
{
	projectname = NULL;
	morph = new CMorph();
	render = FALSE;
	mw = NULL;
	status = STATFMASK;

	createWidgets();
}

void	kmorph::createWidgets()
{
	int		x,y;
	int		w = width,h = height;

	setCaption( "kmorph" );

	Container = new QWidget(this);
	Container->resize(200,30);

	createMenu();

#define LABELW	120
#define LABELH	25
#define LINEDW	270
#define LINEDH	23
#define KLINEW	100
#define KLINEH	23

	QFrame *frame;

	frame = new QFrame(this);
	frame->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	frame->setLineWidth( 2 );
	frame->move(5,35);
	frame->resize(425,135);

	lined[0] = new QLineEdit(this);
	lined[0]->move(150,41);
	lined[0]->resize(LINEDW,23);
	label[0] = new QLabel(lined[0],klocale->translate("Source-picture:"),this);
	label[0]->move(10,40);
	label[0]->resize(LABELW,25);
	label[0]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[1] = new QLineEdit(this);
	lined[1]->move(150,66);
	lined[1]->resize(LINEDW,23);
	label[1] = new QLabel(lined[1],klocale->translate("Destination-picture:"),this);
	label[1]->move(10,65);
	label[1]->resize(LABELW,25);
	label[1]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[2] = new QLineEdit(this);
	lined[2]->move(150,91);
	lined[2]->resize(LINEDW,23);
	lined[2]->setText("data/result.%3i.ppm");
	label[2] = new QLabel(lined[2],klocale->translate("Resultnamemask:"),this);
	label[2]->move(10,90);
	label[2]->resize(LABELW,25);
	label[2]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[3] = new KIntegerLine(this);
	lined[3]->setMaxLength(8);
	lined[3]->move(150,116);
	lined[3]->resize(KLINEW,23);
	lined[3]->setText("0");
	label[3] = new QLabel(lined[3],klocale->translate("Startnumber:"),this);
	label[3]->move(10,115);
	label[3]->resize(LABELW,25);
	label[3]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[4] = new KIntegerLine(this);
	lined[4]->setMaxLength(8);
	lined[4]->move(150,141);
	lined[4]->resize(KLINEW,23);
	lined[4]->setText("10");
	label[4] = new QLabel(lined[4],klocale->translate("Endnumber:"),this);
	label[4]->move(10,140);
	label[4]->resize(LABELW,25);
	label[4]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	frame = new QFrame(this);
	frame->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	frame->setLineWidth( 2 );
	frame->move(5,180);
	frame->resize(255,135);

	lined[5] = new KIntegerLine(this);
	lined[5]->setMaxLength(8);
	lined[5]->move(150,186);
	lined[5]->resize(KLINEW,23);
	lined[5]->setText("0");
	label[5] = new QLabel(lined[5],klocale->translate("Warpstart:"),this);
	label[5]->move(10,185);
	label[5]->resize(LABELW,25);
	label[5]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[6] = new KIntegerLine(this);
	lined[6]->setMaxLength(8);
	lined[6]->move(150,211);
	lined[6]->resize(KLINEW,23);
	lined[6]->setText("10");
	label[6] = new QLabel(lined[6],klocale->translate("Warpend:"),this);
	label[6]->move(10,210);
	label[6]->resize(LABELW,25);
	label[6]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	check[0] = new QCheckBox(this);
	check[0]->move(150,236);
	check[0]->resize(23,23);
	connect(check[0],SIGNAL(toggled(bool)),SLOT(warponlyClicked(bool)));
	label[9] = new QLabel(check[0],klocale->translate("Warp only"),this);
	label[9]->move(10,235);
	label[9]->resize(LABELW,25);
	label[9]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);
	
	lined[7] = new KIntegerLine(this);
	lined[7]->setMaxLength(8);
	lined[7]->move(150,261);
	lined[7]->resize(KLINEW,23);
	lined[7]->setText("0");
	label[7] = new QLabel(lined[7],klocale->translate("Colormixstart:"),this);
	label[7]->move(10,260);
	label[7]->resize(LABELW,25);
	label[7]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	lined[8] = new KIntegerLine(this);
	lined[8]->setMaxLength(8);
	lined[8]->move(150,286);
	lined[8]->resize(KLINEW,23);
	lined[8]->setText("10");
	label[8] = new QLabel(lined[8],klocale->translate("Colormixend:"),this);
	label[8]->move(10,285);
	label[8]->resize(LABELW,25);
	label[8]->setAlignment(ExpandTabs|AlignVCenter|AlignRight);

	reload = new QPushButton(klocale->translate("Reload pictures"),this);
	reload->move(270,135);
	reload->resize(150,25);
	connect(reload,SIGNAL(clicked()),this,SLOT(reloadClicked()));

	stbar = new KStatusBar(this);
	stbar->move(2,320);
	stbar->resize(431,25);
	stbar->insertItem(stloc("No Project!"),1);
	stbar->enable(KStatusBar::Show);
	stbar->show();

	enableMenus();
}

void	kmorph::createMenu()
{
	Mfile = new MPopupMenu;
	CHECK_PTR(Mfile);
	mid[0] = Mfile->insertItem(klocale->translate("New"),this,SLOT(newClicked()),ALT+Key_N);
	mid[1] = Mfile->insertItem(klocale->translate("Open"),this,SLOT(openClicked()),ALT+Key_O);
	mid[2] = Mfile->insertItem(klocale->translate("Save"),this,SLOT(saveClicked()),ALT+Key_S);
	mid[3] = Mfile->insertItem(klocale->translate("Save As"),this,SLOT(saveAsClicked()),0);
	Mfile->insertSeparator();
	mid[4] = Mfile->insertItem(klocale->translate("Open sourcepicture"),this,SLOT(openPicSClicked()),0);
	mid[5] = Mfile->insertItem(klocale->translate("Open dest.picture"),this,SLOT(openPicDClicked()),0);
	Mfile->insertSeparator();
	mid[6] = Mfile->insertItem(klocale->translate("Exit"),this,SLOT(quitClicked()),ALT+Key_Q);

	Medit = new MPopupMenu;
	CHECK_PTR(Medit);
	mid[7] = Medit->insertItem(klocale->translate("Editor"),this,SLOT(editClicked()),ALT+Key_E);
	Medit->insertSeparator();
	mid[8] = Medit->insertItem(klocale->translate("Render"),this,SLOT(renderClicked()),ALT+Key_R);

	Mhelp = new MPopupMenu;
	CHECK_PTR(Mhelp);
	mid[9] = Mhelp->insertItem(klocale->translate("Help"),this,SLOT(helpClicked()),0);
	Mhelp->insertSeparator();
	mid[10] = Mhelp->insertItem(klocale->translate("About"),this,SLOT(aboutClicked()),0);

	mainmenu = new KMenuBar(Container,"mainmenu");
	CHECK_PTR(mainmenu);
	mainmenu->insertItem(klocale->translate("&File"),Mfile);
	mainmenu->insertItem(klocale->translate("&Edit"),Medit);
	mainmenu->insertSeparator();
	mainmenu->insertItem(klocale->translate("&Help"),Mhelp);
}

void	kmorph::enableMenus()
{
	int	t;

	for(t = 0;t < 7;t++) Mfile->setItemEnabled(mid[t],TRUE);
	for(t = 7;t < 9;t++) Medit->setItemEnabled(mid[t],TRUE);
	Medit->changeItem(klocale->translate("Editor"),mid[7]);
	Medit->changeItem(klocale->translate("Render"),mid[8]);
	if((status & (STATSPIC | STATDPIC | STATFMASK)) != (STATSPIC | STATDPIC | STATFMASK))
	{
		Medit->setItemEnabled(mid[8],FALSE);
	}
	if((status & (STATSPIC | STATDPIC)) != (STATSPIC | STATDPIC))
	{
		Medit->setItemEnabled(mid[7],FALSE);
	}
	for(t = 9;t < 11;t++) Mhelp->setItemEnabled(mid[t],TRUE);
	reload->setEnabled(TRUE);
}

void	kmorph::disableMenusForRender()
{
	int	t;

	for(t = 0;t < 6;t++) Mfile->setItemEnabled(mid[t],FALSE);
	for(t = 7;t < 8;t++) Medit->setItemEnabled(mid[t],FALSE);
	Medit->changeItem(klocale->translate("Stop"),mid[8]);
	reload->setEnabled(FALSE);
}

void	kmorph::disableMenusForEdit()
{
	int	t;

	for(t = 0;t < 2;t++) Mfile->setItemEnabled(mid[t],FALSE);
	for(t = 4;t < 6;t++) Mfile->setItemEnabled(mid[t],FALSE);
	if(status != (STATSPIC | STATDPIC | STATFMASK))
	{
		Medit->setItemEnabled(mid[8],FALSE);
	}
	for(t = 7;t < 9;t++) Medit->setItemEnabled(mid[t],FALSE);
	reload->setEnabled(FALSE);
}

void	kmorph::newClicked()
{
	lined[0]->setText("");
	lined[1]->setText("");
	lined[2]->setText("");
	lined[3]->setText("0");
	lined[4]->setText("10");
	lined[5]->setText("0");
	lined[6]->setText("10");
	lined[7]->setText("0");
	lined[8]->setText("10");
	check[0]->setChecked(FALSE);
	if(projectname) free(projectname);
	projectname = NULL;
	while(morph->getNPoint() > 0) morph->delPoint(0);
	morph->setSrcPicture(NULL,0,0);
	morph->setDestPicture(NULL,0,0);
	morph->setResultPicture(NULL,0,0);
	stbar->changeItem(stloc("New Project!"),1);
}

void	kmorph::openClicked()
{
	FILE		*fp;
	int		t,onlywarp,si,ei,fs,fe,points,xa,ya,xb,yb;
	char		line[1024];
	char		name[1024];
	char		str[1024];
	KFileDialog	*fd;
	QString		qs;
	byte		*picdata = NULL;
	int		w,h;

	fd = new KFileDialog();

	fd->exec();
	qs = fd->getSelectedFile();
	strcpy(name,qs.data());
	delete fd;

	if((fp = fopen(name,"r")) == NULL)
	{
		stbar->changeItem(stloc("Cannot open file!"),1);
		return;
	}
	do
	{
		fgets(line,1024,fp);
	}
	while(line[0] == '#');
	if(strncmp(line,"kmorph",5) != 0)
	{
		fclose(fp);
		stbar->changeItem(stloc("Is not a kmorph-file!"),1);
		return;
	}

	if(projectname) free(projectname);
	projectname = strdup(name);
	delete morph;
	morph = new CMorph();

	fgets(line,1024,fp);	
	strncpy(str,line,511);
	str[strlen(str) - 1] = '\0';
	lined[0]->setText(str);
	fgets(line,1024,fp);
	strncpy(str,line,511);
	str[strlen(str) - 1] = '\0';
	lined[1]->setText(str);
	fgets(line,1024,fp);
	strncpy(str,line,511);
	str[strlen(str) - 1] = '\0';
	lined[2]->setText(str);
	fgets(line,1024,fp);
	sscanf(line,"%i %i %i %i",&si,&ei,&fs,&fe);
	sprintf(str,"%i",si);lined[3]->setText(str);
	sprintf(str,"%i",ei);lined[4]->setText(str);
	sprintf(str,"%i",fs);lined[5]->setText(str);
	sprintf(str,"%i",fe);lined[6]->setText(str);
	sprintf(str,"%i",fs);lined[7]->setText(str);
	sprintf(str,"%i",fe);lined[8]->setText(str);
	fgets(line,1024,fp);
	sscanf(line,"%i",&onlywarp);
	check[0]->setChecked(onlywarp);
	fgets(line,1024,fp);
	sscanf(line,"%i",&points);
	for(t = 0;t < points;t++)
	{
		fgets(line,1024,fp);
		sscanf(line,"%i %i %i %i\n",&xa,&ya,&xb,&yb);
		morph->addPoint(xa,ya,xb,yb);
	}
	fclose(fp);
	if(loadPic((char*)lined[0]->text(),&picdata,&w,&h))
	{
		morph->setSrcPicture(picdata,w,h);
		status |= STATSPIC;
		if(picdata) free(picdata);
		picdata = NULL;
	}
	if(loadPic((char*)lined[1]->text(),&picdata,&w,&h))
	{
		morph->setDestPicture(picdata,w,h);
		status |= STATDPIC;
		if(picdata) free(picdata);
		picdata = NULL;
	}
	w = max(morph->getDestPicture()->getWidth(),morph->getSrcPicture()->getWidth());
	h = max(morph->getDestPicture()->getHeight(),morph->getSrcPicture()->getHeight());
	morph->setResultPicture((picdata = (byte*)malloc(w * h * 3)),w,h);
	if(picdata) free(picdata);

	stbar->changeItem(stloc("Project loaded!"),1);
	enableMenus();
}

void	kmorph::saveClicked()
{
	FILE		*fp;
	int		t,points,xa,ya,xb,yb;

	if(projectname == NULL)
	{
		saveAsClicked();
		return;
	}
	if((fp = fopen(projectname,"w")) == NULL)
	{
		stbar->changeItem(stloc("Cannot open project for writing!"),1);
		return;
	}
	fprintf(fp,"# Projectfile for kmorph V%s\n",VERSION);
	fprintf(fp,"kmorph\n");
	fprintf(fp,"%s\n",lined[0]->text());
	fprintf(fp,"%s\n",lined[1]->text());
	fprintf(fp,"%s\n",lined[2]->text());
	fprintf(fp,"%i %i %i %i\n",atoi(lined[3]->text()),atoi(lined[4]->text()),atoi(lined[5]->text()),atoi(lined[6]->text()));
	fprintf(fp,"%i\n",check[0]->isChecked());
	fprintf(fp,"%i\n",(points = morph->getNPoint()));
	for(t = 0;t < points;t++)
	{
		morph->getPoint(t,&xa,&ya,&xb,&yb);
		fprintf(fp,"%i %i %i %i\n",xa,ya,xb,yb);
	}
	fclose(fp);
	stbar->changeItem(stloc("Project saved!"),1);
}

void	kmorph::saveAsClicked()
{
	QString		qs;
	char		name[1024];
	KFileDialog	*fd;
	int		t,points,xa,ya,xb,yb;
	FILE		*fp;

	fd = new KFileDialog();

	fd->exec();
	qs = fd->getSelectedFile();
	strcpy(name,qs.data());
	delete fd;

	if((fp = fopen(name,"w")) == NULL)
	{
		stbar->changeItem(stloc("Cannot open project for writing!"),1);
		return;
	}
	fprintf(fp,"# Projectfile for kmorph V%s\n",VERSION);
	fprintf(fp,"kmorph\n");
	fprintf(fp,"%s\n",lined[0]->text());
	fprintf(fp,"%s\n",lined[1]->text());
	fprintf(fp,"%s\n",lined[2]->text());
	fprintf(fp,"%i %i %i %i\n",atoi(lined[3]->text()),atoi(lined[4]->text()),atoi(lined[5]->text()),atoi(lined[6]->text()));
	fprintf(fp,"%i\n",check[0]->isChecked());
	fprintf(fp,"%i\n",(points = morph->getNPoint()));
	for(t = 0;t < points;t++)
	{
		morph->getPoint(t,&xa,&ya,&xb,&yb);
		fprintf(fp,"%i %i %i %i\n",xa,ya,xb,yb);
	}
	fclose(fp);
	stbar->changeItem(stloc("Project saved!"),1);
	if(projectname) free(projectname);
	projectname = strdup(name);
}

int	kmorph::loadPic(char *name,byte **pic,int *w,int *h)
{
	FILE		*fp;
	char		line[256];

	if(*pic) free(*pic);
	if((fp = fopen(name,"r")) == NULL)
	{
		sprintf(line,"%s %s!",klocale->translate("Cannot open file:"),name);
		stbar->changeItem(line,1);
		return FALSE;
	}
	fgets(line,sizeof(line),fp);
	if(strncmp(line,"P6",2) != 0)
	{
		sprintf(line,"%s %s %s",klocale->translate("File"),name,klocale->translate("is no PPM format!"));
		stbar->changeItem(line,1);
		return FALSE;
	}
	do
	{
		fgets(line,sizeof(line),fp);
	}
	while(line[0] == '#');
	*w = atoi(line);
	*h = atoi(line + 4);
	fgets(line,sizeof(line),fp);
	if((*pic = (byte *)malloc(*w * *h * 3)) == NULL)
	{
		stbar->changeItem(stloc("Out of memory!"),1);
		fclose(fp);
		return FALSE;
	}
	fread(*pic,*w * *h,3,fp);
	fclose(fp);

	return TRUE;
}

int	kmorph::savePic(char *name,byte *pic,int w,int h)
{
	FILE		*fp;
	char		line[256];

	if(pic == NULL) return -1;
	if((fp = fopen(name,"w")) == NULL)
	{
		sprintf(line,"%s %s",klocale->translate("Cannot open file for writing:"),name);
		stbar->changeItem(line,1);

		return -1;
	}
	fprintf(fp,"P6\n");
	fprintf(fp,"# Erzeugt mit kmorph V");
	fprintf(fp,VERSION);
	fprintf(fp,"\n");
	fprintf(fp,"%i %i\n255\n",w,h);
	fwrite(pic,w * h,3,fp);
	fclose(fp);

	return 0;
}

void	kmorph::openPicSClicked()
{
	QString		qs;
	char		name[1024];
	KFileDialog	*fd;
	int		w,h;
	byte		*picdata = NULL;

	fd = new KFileDialog();

	fd->exec();
	qs = fd->getSelectedFile();
	strcpy(name,qs.data());
	delete fd;

	if(loadPic(name,&picdata,&w,&h))
	{
		lined[0]->setText(name);
		morph->setSrcPicture(picdata,w,h);
		status |= STATSPIC;
		if(picdata) free(picdata);
	}
	w = max(morph->getDestPicture()->getWidth(),morph->getSrcPicture()->getWidth());
	h = max(morph->getDestPicture()->getHeight(),morph->getSrcPicture()->getHeight());
	morph->setResultPicture((picdata = (byte*)malloc(w * h * 3)),w,h);
	if(picdata) free(picdata);
	enableMenus();
}

void	kmorph::openPicDClicked()
{
	QString		qs;
	char		name[1024];
	KFileDialog	*fd;
	int		w,h;
	byte		*picdata = NULL;

	fd = new KFileDialog();

	fd->exec();
	qs = fd->getSelectedFile();
	strcpy(name,qs.data());
	delete fd;

	if(loadPic(name,&picdata,&w,&h))
	{
		lined[1]->setText(name);
		morph->setDestPicture(picdata,w,h);
		status |= STATDPIC;
		if(picdata) free(picdata);
	}
	w = max(morph->getDestPicture()->getWidth(),morph->getSrcPicture()->getWidth());
	h = max(morph->getDestPicture()->getHeight(),morph->getSrcPicture()->getHeight());
	morph->setResultPicture((picdata = (byte*)malloc(w * h * 3)),w,h);
	if(picdata) free(picdata);
	enableMenus();
}

void	kmorph::reloadClicked()
{
	int		w,h,s,d;
	byte		*picdata = NULL;

	if(loadPic((char*)lined[0]->text(),&picdata,&w,&h))
	{
		morph->setSrcPicture(picdata,w,h);
		status |= STATSPIC;
		if(picdata) free(picdata);
		picdata = NULL;
		s = TRUE;
	}
	else
	{
		stbar->changeItem(stloc("Cannot reload sourcepicture!"),1);
		s = FALSE;
	}
	if(loadPic((char*)lined[1]->text(),&picdata,&w,&h))
	{
		morph->setDestPicture(picdata,w,h);
		status |= STATDPIC;
		if(picdata) free(picdata);
		picdata = NULL;
		d = TRUE;
	}
	else
	{
		stbar->changeItem(stloc("Cannot reload destinationpicture!"),1);
		d = FALSE;
	}
	w = max(morph->getDestPicture()->getWidth(),morph->getSrcPicture()->getWidth());
	h = max(morph->getDestPicture()->getHeight(),morph->getSrcPicture()->getHeight());
	morph->setResultPicture((picdata = (byte*)malloc(w * h * 3)),w,h);
	if(picdata) free(picdata);

	if(s && d) stbar->changeItem(stloc("Pictures reloaded!"),1);

}	enableMenus();


void	kmorph::winDestroyed()
{
	enableMenus();
	mw = NULL;
}

void	kmorph::editClicked()
{
	if(mw == NULL)
	{
		disableMenusForEdit();
		mw = new kmorphwin(0,"",morph);
		mw->resize(500,350);
		mw->show();
		connect(mw,SIGNAL(destroyed()),SLOT(winDestroyed()));
	}
	else
	{
		enableMenus();
		mw->close();
		delete mw;
		mw = NULL;
	}
}

void	kmorph::renderClicked()
{
	int	s,e;

	if(render)
	{
		render = FALSE;
		killTimers();
		enableMenus();
		stbar->changeItem(stloc("Rendering stopped!"),1);
		return;
	}
	render = TRUE;
	disableMenusForRender();

	s = atoi(lined[3]->text());
	e = atoi(lined[4]->text());

	morph->startRender(s,e);

	startTimer(50);
}

void	kmorph::timerEvent(QTimerEvent *te)
{
	char	str[50];
	int	f,s,e,l,n;

	if(morph->isRendering())
	{
		if(morph->frameReady())
		{
			char		name[1024],mask[1024];
			CPicture	*P;

			killTimers();
			strcpy(mask,lined[2]->text());
			sprintf(name,mask,morph->getRenderFrame() + atoi(lined[3]->text()) - 1);
			P = morph->getResultPicture();
			savePic(name,P->getPicture(),P->getWidth(),P->getHeight());
			startTimer(50);
		}

		s = atoi(lined[7]->text());
		e = atoi(lined[8]->text());
		morph->setColormorphIndex(s,e);

		s = atoi(lined[5]->text());
		e = atoi(lined[6]->text());
		morph->setMorphIndex(s,e);

		s = atoi(lined[3]->text());
		e = atoi(lined[4]->text());

		morph->renderFrameLine();
		l = morph->getRenderLine();
		n = morph->getRenderLines();
		f = morph->getRenderFrame();

		sprintf(str,"%s %i %s %i  %s %i %s %i",stloc("Render frame"),f - s + 1,stloc("of"),e - s + 1,stloc("line"),l + 1,stloc("of"),n + 1);
		stbar->changeItem(str,1);
	}
	else
	{
		killTimers();
		render = FALSE;
		stbar->changeItem(stloc("Render done!"),1);
	}
}

void	kmorph::quitClicked()
{
	exit(0);
}

void	kmorph::helpClicked()
{
	QString		str,topic;

/*
	str = "kmorph V" VERSION " \n\n";
	str += klocale->translate("Help not available yet!");
	KMsgBox	mb(0,stloc("Help"),str,1,stloc("Sorry!"));
	mb.show();
*/
	str = "kmorph/";
	str += klocale->translate("kmorph.html");
	kapp->invokeHTMLHelp(str,topic);
}

void	kmorph::aboutClicked()
{
	QString		str;

	str = "kmorph V" VERSION " \n\n";
	str += klocale->translate("Author:");
	str += "Tobias Wollgam\n\neMail: wollgam@cww.de";
	KMsgBox	mb(0,stloc("About"),str,1,stloc("Quit"));
	mb.show();
}

void	kmorph::warponlyClicked(bool on)
{
	lined[7]->setEnabled(!on);
	lined[8]->setEnabled(!on);
	morph->setWarpOnly(on);
}

