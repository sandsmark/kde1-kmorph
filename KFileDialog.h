#ifndef KFileDialog_included
#define KFileDialog_included

#undef Unsorted
#include <qdir.h>
#include <qdialog.h>
#include <qlistbox.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qcombo.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <kapp.h>

#define Unsorted 0

class KFileDialog : public QDialog
{

    Q_OBJECT

  private:

    QListBox    *listbox1;
    QPushButton *openknob1;
    QPushButton *rescanknob1;
    QPushButton *cancelknob1;
    QComboBox   *comboknob1;
    QPushButton *parentknob1;
    QPushButton *homeknob1;
    QLineEdit   *diredit1;
    QLineEdit   *fileedit1;
    QBoxLayout  *vertilayout1,
                *horizlayout1,
                *horizlayout2,
                *horizlayout3,
                *horizlayout4,
                *horizlayout5;
    KDNDDropZone *dropzone;
                
    // These things should be remembered when the user repops this viewer
    int _window_x,
        _window_y,
        _window_width,
        _window_height; // x,y, width, height
        
    QDir        *d;

  public:

    KFileDialog  (  QWidget* _parent=0,  const char* _name=0, bool _modal=TRUE );
    ~KFileDialog();

   static QString getOpenFileName();
   static QString getSaveFileName();

   QString getSelectedFile();

  public slots:

   void scanDir ( );
   void gotoHome ( );
   void gotoParent ( );
   void gotoDir ( );
   void fileChosen ( int  );
   void fileSelected ( int  );
   void dropEvent ( KDNDDropZone *);
};

#endif // KFileDialog_included
