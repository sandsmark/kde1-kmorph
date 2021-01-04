#include "KFileDialog.moc"

/*
 *
 * It's not the nicest implementation but it works allright,
 * Maybe in next versions I'll better the thing
 *
 */

#include <iostream>
#include <kapp.h>
#include <kiconloader.h>
#include <stdlib.h>

KFileDialog::KFileDialog (QWidget* _parent,const char* _name, bool _modal):QDialog( _parent, _name, _modal)
{

  QString kdehome(getenv("KDEDIR"));
//  QString kdehome = KApplication::kdedir();
  _window_width = 270;
  _window_height = 400;

  vertilayout1 = new QBoxLayout(this,QBoxLayout::TopToBottom,5,5);

  horizlayout1 = new QBoxLayout(QBoxLayout::RightToLeft);
  horizlayout2 = new QBoxLayout(QBoxLayout::LeftToRight);
  horizlayout3 = new QBoxLayout(QBoxLayout::RightToLeft);
  horizlayout4 = new QBoxLayout(QBoxLayout::RightToLeft);
  horizlayout5 = new QBoxLayout(QBoxLayout::LeftToRight);

  vertilayout1->addLayout( horizlayout1, 10 );
  vertilayout1->addLayout( horizlayout2, 0 );
  vertilayout1->addLayout( horizlayout3, 0 );
  vertilayout1->addLayout( horizlayout4, 0 );
  vertilayout1->addLayout( horizlayout5, 0 );

  listbox1 = new QListBox( this, "ListBox_1" );
  listbox1->setGeometry( 5, 5, 235, 200 );
  listbox1->setFrameStyle( 51 );
  listbox1->setLineWidth( 2 );
  listbox1->setMinimumSize(listbox1->size());
  listbox1->setFont(QFont("courier",12));
  connect(listbox1,SIGNAL(highlighted(int)),this,SLOT(fileSelected(int)));
  connect(listbox1,SIGNAL(selected(int)),this,SLOT(fileChosen(int)));

  comboknob1 = new QComboBox( FALSE, this, "ComboBox_1" );
  comboknob1->setGeometry( 130, 210, 110, 30 );
  comboknob1->setSizeLimit( 30 );
  comboknob1->setMinimumSize(comboknob1->size());
  
  parentknob1 = new QPushButton( this, "PushButton_4" );
  parentknob1->setGeometry( 95, 210, 30, 30 );
  parentknob1->setText( "P" );
  parentknob1->setPixmap(ICON("up.xpm"));
  parentknob1->setMaximumSize(parentknob1->size());
  parentknob1->setMinimumSize(parentknob1->size());
  connect(parentknob1,SIGNAL(clicked()),this,SLOT(gotoParent()));
  QToolTip :: add(parentknob1,"Parent");

  homeknob1 = new QPushButton( this, "PushButton_5" );
  homeknob1->setGeometry( 60, 210, 30, 30 );
  homeknob1->setText( "H" );
  homeknob1->setPixmap(ICON("home.xpm"));
  homeknob1->setMaximumSize(homeknob1->size());
  homeknob1->setMinimumSize(homeknob1->size());
  connect(homeknob1,SIGNAL(clicked()),this,SLOT(gotoHome()));
  QToolTip :: add(homeknob1,"Home");

  diredit1 = new QLineEdit( this, "LineEdit_1" );
  diredit1->setGeometry( 60, 245, 180, 25 );
  diredit1->setText( "" );
  diredit1->setMinimumSize(diredit1->size());
  connect (diredit1,SIGNAL(returnPressed()),this,SLOT(gotoDir()));

  fileedit1 = new QLineEdit( this, "LineEdit_2" );
  fileedit1->setGeometry( 60, 275, 180, 25 );
  fileedit1->setText( "" );
  fileedit1->setMinimumSize(fileedit1->size());

  openknob1 = new QPushButton( this, "PushButton_1" );
  openknob1->setGeometry( 5, 305, 55, 30 );
  openknob1->setText( "Open" );
  openknob1->setMinimumSize(openknob1->size());
  connect(openknob1,SIGNAL(clicked()),this,SLOT(accept()));

  rescanknob1 = new QPushButton( this, "PushButton_2" );
  rescanknob1->setGeometry( 65, 305, 55, 30 );
  rescanknob1->setText( "rescan" );
  rescanknob1->setMinimumSize(rescanknob1->size());
  connect (rescanknob1,SIGNAL(clicked()),this,SLOT(scanDir()));

  cancelknob1 = new QPushButton( this, "PushButton_3" );
  cancelknob1->setGeometry( 185, 305, 55, 30 );
  cancelknob1->setText( "Cancel" );
  cancelknob1->setAutoDefault( TRUE );
  cancelknob1->setMinimumSize(cancelknob1->size());
  connect(cancelknob1,SIGNAL(clicked()),this,SLOT(reject()));

  QPushButton* tmpQPushButton;
  tmpQPushButton = new QPushButton( this, "PushButton_6" );
  tmpQPushButton->setGeometry( 125, 305, 55, 30 );
  tmpQPushButton->setText( "no" );
  tmpQPushButton->setMinimumSize(tmpQPushButton->size());

  horizlayout5->addWidget(openknob1,10,AlignCenter);  
  horizlayout5->addWidget(rescanknob1,10,AlignCenter);  
  horizlayout5->addWidget(tmpQPushButton,10,AlignCenter);  
  horizlayout5->addWidget(cancelknob1,10,AlignCenter);  

  horizlayout4->addWidget(fileedit1,0,AlignCenter);  

  horizlayout3->addWidget(diredit1,0,AlignCenter);  

  horizlayout2->addWidget(parentknob1,1,AlignCenter);  
  horizlayout2->addWidget(homeknob1,1,AlignCenter);  
  horizlayout2->addWidget(comboknob1,1,AlignCenter);  

  horizlayout1->addWidget(listbox1,0,AlignCenter);

  vertilayout1->activate();
  
  dropzone = new KDNDDropZone(this,DndURL);
  connect ( dropzone,SIGNAL(dropAction(KDNDDropZone *)),this,SLOT(dropEvent(KDNDDropZone *)));

  d = new QDir((QDir :: currentDirPath()).data(),
                        NULL, QDir::Name| QDir::DirsFirst| QDir::IgnoreCase,
                        QDir::All| QDir::Hidden | QDir:: System);
  scanDir();

  resize(_window_width,_window_height);


}

QString KFileDialog :: getSelectedFile()
{
  QString t = diredit1->text();
  t = t + '/';
  t = t + fileedit1->text();
  return t;
}

QString KFileDialog :: getOpenFileName()
{
  QString res;
  KFileDialog kfdlg(NULL,"filedlg");

  kfdlg.setCaption("Pick file to open");

  if (kfdlg.exec())
    res = kfdlg.getSelectedFile();
  return res;

}

QString KFileDialog :: getSaveFileName()
{
  QString res;
  KFileDialog kfdlg(NULL,"filedlg");

  kfdlg.setCaption("Pick file to save");

  if (kfdlg.exec())
    res = kfdlg.getSelectedFile();
  return res;
}

void KFileDialog :: scanDir( )
{
  char fnamebuf[200];
  char itemstring[300];

  diredit1->setText(d->path());

  listbox1->setUpdatesEnabled(FALSE);
  listbox1->clear();

  if (d->isReadable())
  {
    const QFileInfoList *list = d->entryInfoList();
    QFileInfoListIterator it( *list );
    QFileInfo *fi;

    while ( (fi=it.current()) )
    {
      ++it;     // goto next list element
      if ( fi->fileName() == ".." || fi->fileName() == "." )
        continue;
      strcpy(fnamebuf,fi->fileName().data());
      if (fi->isDir())
        strcat(fnamebuf,"/");
  //    else if (fi->isExecutable())
  //      strcat(fnamebuf,"*");
  //    else if (fi->isSymLink())
  //      strcat(fnamebuf,"~");

      sprintf( itemstring,"%-20s  %10d %-8s",
                   fnamebuf,
                   fi->size(),
                   fi->owner() );

      listbox1->insertItem(strdup(itemstring));
    }
  }
  listbox1->setUpdatesEnabled(TRUE);
  listbox1->repaint();

}

void KFileDialog :: fileChosen ( int i )
{
  QString s = listbox1->text(i);
  s = s.left(s.find(' '));
  s = s.left(s.find('/'));

  QFileInfo f (QDir(diredit1->text()),s.data());

  if (f.isDir())
  {
    d->cd(s.data());
    scanDir();
    fileedit1->setText("");
  }
  else if (f.isFile())
  {  // Set the current file string to this file
    accept();
  }
    
}

void KFileDialog :: fileSelected ( int i )
{
  QString s = listbox1->text(i);
  s = s.left(s.find(' '));
  s = s.left(s.find('/'));

  fileedit1->setText(s.data());
}

void KFileDialog :: gotoDir ( )
{
  // Dir edit box clicked
  d->setPath(diredit1->text());
  fileedit1->setFocus();
  scanDir();
  fileedit1->setText("");
}


void KFileDialog :: gotoHome ( )
{
  // Home button clicked
  
//  cout << path << "\n";
//  diredit1->setText(d->homeDirPath().data()); 
  d->setPath(d->homeDirPath().data());
  
  scanDir();
  fileedit1->setText("");
}

void KFileDialog :: gotoParent (  )
{
  // Parent button clicked
  if ( d->cdUp() )
  {
    scanDir();
    fileedit1->setText("");
  }
}

void KFileDialog :: dropEvent ( KDNDDropZone *dropz )
{

  QStrList ss = dropz->getURLList();
  QString s = ss.first();
  s = s.right(s.length()-s.find(':')-1);

  QFileInfo f(s.data());
  if (f.isDir())
  {
    d->setPath(s.data());
    scanDir();
    fileedit1->setText("");
  } else if (f.isFile())
  {
    int pos = s.findRev('/');
    d->setPath( s.left(pos) );
    scanDir();  
    fileedit1->setText(s.right(s.length()-pos-1));
  }

}

KFileDialog::~KFileDialog()
{
  delete d;
  delete listbox1;
  delete openknob1;
  delete rescanknob1;
  delete cancelknob1;
  delete parentknob1;
  delete homeknob1;
  delete comboknob1;
  delete diredit1;
  delete horizlayout1;
  delete horizlayout2;
  delete horizlayout3;
  delete horizlayout4;
  delete horizlayout5;
  delete vertilayout1;
}

