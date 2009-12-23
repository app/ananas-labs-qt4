#include "deditaction.h"

#include <qvariant.h>
#include <qimage.h>
#include <qpixmap.h>

#include <qstatusbar.h>
//Added by qt3to4:
#include <QPixmap>
#include "acfg.h"
#include <qtabbar.h>
//#include <qsinterpreter.h>
//#include <qseditor.h>
#include <scriptedit.h>

/*
 *  Constructs a dEditAction as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
dEditAction::dEditAction(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
    setupUi(this);

    (void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
dEditAction::~dEditAction()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void dEditAction::languageChange()
{
    retranslateUi(this);
}

void dEditAction::setData( ActionListViewItem * o )
{
    item = o;
    md = o->md;
    obj = o->obj;
    aCfgItem active, passive;
    QString st;
    QPixmap pix, pix1;
    static const char *empty[] = {
 "1 1 1 1",
 ". c None",
 "."};
    dp = new QPixmap( empty );

    aAliasEditor *a = new aAliasEditor( md, obj, tAliases );
    al = a;
    al->setData();  // alias table init

    pActive->setPixmap( *dp ); // set empty pixmaps
    pPassive->setPixmap( *dp );
    eName->setText( md->attr( obj, mda_name ) );
    setCaption( tr("Action:") + md->attr( obj, mda_name ) );
    eDescription->setText( md->sText( obj, md_description ) );
    eParams->setText( md->sText( obj, md_argument ) ); // set atributes
    st = md->attr ( obj, mda_type);
    if ( st == "0" ) cbType->setCurrentItem( 0 );
    else if ( st == "1" ) cbType->setCurrentItem( 1 );
    eModule->setPlainText( md->sText( obj, md_sourcecode ) );
    active = md->findChild( obj, md_active_picture, 0 );
    if ( !active.isNull() ) {     // set active mode picture if exists
 bRemoveActive->setEnabled( TRUE );
 pix.loadFromData( md->binary( active ) );
 pActive->setPixmap( pix );
 setIcon( pix );
    }
    passive = md->findChild( obj, md_passive_picture, 0 );
    if ( !passive.isNull() ) {     // set passive mode picture if exists
 bRemovePassive->setEnabled( TRUE );
 pix1.loadFromData( md->binary( passive ) );
 pPassive->setPixmap( pix1 );
    }

    QStringList tlist = md->ObjectTypes();
    otypes.clear();
    cbObject->clear();
    for ( QStringList::Iterator it = tlist.begin(); it != tlist.end(); ++it )
    {
 otypes.append( (*it).section( "\t", 0, 0 ).section(" ", 1, 1 ) );
 cbObject->insertItem( (*it).section("\t", 1, 1 ));
    }
 QString oid = md->sText( obj, md_objectid );
 uint n = otypes.count();
 for ( uint i = 0; i < n; i++ )
  if ( otypes[i] == oid )
  {
   cbObject->setCurrentItem( i );
   break;
  }
 cbObjectChange();
 n = formslist.count();
 oid = md->sText( obj, md_formid );
 for ( uint i = 0; i < n; i++ ){
  if ( formslist[i] == oid )
  {
   cbForm->setCurrentItem( i );
   break;
  }

 }
 int type = md->sText( obj, md_actiontype ).toInt();
 switch ( type )
 {
  case 0 :
   cbAction->setCurrentItem(md_action_new);
   break;
  case 1 :
   cbAction->setCurrentItem(md_action_edit);
   break;
  case 2 :
   cbAction->setCurrentItem(md_action_view);
   break;
  case 3 :
   cbAction->setCurrentItem(md_action_execute);
   break;
 }


    cbTypeChange();
}

void dEditAction::updateMD()
{
 al->updateMD( );
 char sid[50]="";
 int id = 0;

 item->setText( 0, eName->text() );
 md->setAttr( obj, mda_name, eName->text() );
 md->setSText( obj, md_description, eDescription->text() );
 md->setSText( obj, md_argument, eParams->text() );
 if ( cbType->currentItem() == 0 )
 {
     md->setAttr( obj, mda_type, "0" );
     id=cbAction->currentItem();
  switch ( id )
  {
   case 0 :
   sprintf(sid, "%i", md_action_new);
   md->setSText( obj, md_actiontype, sid );
   break;
   case 1 :
   sprintf(sid, "%i", md_action_edit );
   md->setSText( obj, md_actiontype, sid );
   break;
   case 2 :
   sprintf(sid, "%i", md_action_view);
   md->setSText( obj, md_actiontype, sid );
   break;
   case 3 :
   sprintf(sid, "%i", md_action_execute);
   md->setSText( obj, md_actiontype, sid );
   break;
  }
     id=cbObject->currentItem();
     md->setSText( obj, md_objectid, otypes[id] );
  id=cbForm->currentItem();
  md->setSText( obj, md_formid, formslist[id] );
 }

 if ( cbType->currentItem() == 1 )
 {
     md->setAttr( obj, mda_type, "1" );
     md->setSText( obj, md_sourcecode, eModule->toPlainText() );
 }
}


void dEditAction::init()
{

    delete statusBar();
    //eModule->setInterpreter(new QSInterpreter());
//    tabWidget2->page(1)->hide() ;
//    tabWidget2->setTabEnabled ( tabWidget2->page(1), FALSE );
}


void dEditAction::destroy()
{
    updateMD();
    ( (MainForm*)this->topLevelWidget() )->wl->remove( this );
    ( (MainForm*)this->topLevelWidget() )->removeTab(name());
}


void dEditAction::cbTypeChange ()
{
//    if ( cbType->currentItem() == 0 )
// selectStandart();
 //   if ( cbType->currentItem() == 1 )
// selectUser();
// if ( cbType->currentItem() == 2 ) wStack->raiseWidget( 2 );
 wStack->raiseWidget( cbType->currentItem() );
}

void dEditAction::selectStandart()
{
    lAction->show();
    lObject->show();
    lParams->show();
    cbAction->show();
    cbObject->show();
    cbForm->show();
    eParams->show();
//    tabWidget2->page(1)->hide() ;
    eModule->hide() ;
//    tabWidget2->setTabEnabled ( tabWidget2->page(1), FALSE );
}



void dEditAction::selectUser()
{

    lAction->hide();
    lObject->hide();
    lParams->hide();
    cbAction->hide();
    cbObject->hide();
    cbForm->hide();
    eParams->hide();
    eModule->show() ;
//    tabWidget2->setTabEnabled ( tabWidget2->page(1), TRUE );
}


void dEditAction::bSetActive_clicked()
{
    QString st;
    QPixmap pix;
//    QFileDialog *fd = new QFileDialog( "", "Images (*.png *.xpm *.jpg *.jpeg *.bmp)", 0, 0, TRUE );
    QByteArray ba;
    QString name;
    QFile f;
//    PixmapPreview *p = new PixmapPreview;
    aCfgItem active;

//    fd->setContentsPreview( p, p );
//    fd->setPreviewMode( QFileDialog::Contents );
//    fd->setContentsPreviewEnabled( TRUE );  // prepair file dialog
//    fd->setCaption( tr("Open image dialog" ) );
//    fd->setMode( QFileDialog::ExistingFiles );
 name= Q3FileDialog::getOpenFileName("", "Images (*.png *.xpm *.jpg *.jpeg *.bmp)", this,
   tr("Open image dialog"),
   tr("Choose an image") );
//    if ( fd->exec() == QDialog::Accepted ) { // if accepted
// name = fd->selectedFile(); // get filename
 if ( !name.isEmpty() ) { // if not empty
     f.setName( name );
     if ( f.open( QIODevice::ReadOnly ) ){ // open file
  ba = f.readAll(); // read data
  f.close();  // close file
  pix.loadFromData( ba );  // load data into pixmap
  active = item->md->findChild( item->obj, md_active_picture, 0 );
  if ( active.isNull() ) active = item->md->insert( item->obj, md_active_picture, QString::null, -1 );
  item->md->setBinary( active, ba ); // save data into cfg
  item->setPixmap( 0, pix );
  setIcon( pix ); // put on caption
     }
 }
// delete fd; // remove filedialog
 pActive->setPixmap( pix ); // put on form
 bRemoveActive->setEnabled( TRUE ); // enable remove button
//    }
}


void dEditAction::bSetPassive_clicked()
{
    QString st;
    QPixmap pix;
    Q3FileDialog *fd = new Q3FileDialog( "", "Images (*.png *.xpm *.jpg *.jpeg *.bmp)", 0, 0, TRUE );
    QByteArray ba;
    QString name;
    QFile f;
    PixmapPreview *p = new PixmapPreview;
    aCfgItem passive;

    fd->setContentsPreview( p, p );
    fd->setPreviewMode( Q3FileDialog::Contents );
    fd->setContentsPreviewEnabled( TRUE );
    fd->setCaption( tr("Open image dialog" ) );
    fd->setMode( Q3FileDialog::ExistingFiles );
    if ( fd->exec() == QDialog::Accepted ) {
 name = fd->selectedFile();
 if ( !name.isEmpty() ) {
     f.setName( name );
     if ( f.open( QIODevice::ReadOnly ) ){
  ba = f.readAll();
  f.close();
  pix.loadFromData( ba );
  passive = item->md->findChild( item->obj, md_passive_picture, 0 );
  if ( passive.isNull() ) passive = item->md->insert( item->obj, md_passive_picture, QString::null, -1 );
  item->md->setBinary( passive, ba );
     }
 }
 delete fd;
 pPassive->setPixmap( pix );
 bRemovePassive->setEnabled( TRUE );
    }
}


void dEditAction::bRemoveActive_clicked()
{
    item->md->remove( item->md->findChild( item->obj, md_active_picture, 0 ) );
    bRemoveActive->setEnabled( FALSE );
    pActive->clear();
    item->setPixmap( 0, 0 );
    setIcon( *dp );
    pActive->setPixmap( *dp );
}


void dEditAction::bRemovePassive_clicked()
{
    item->md->remove( item->md->findChild( item->obj, md_passive_picture, 0 ) );
    bRemovePassive->setEnabled( FALSE );
    pPassive->clear();
    pPassive->setPixmap( *dp );
}


void dEditAction::cbObjectChange()
{
    aCfgItem cobj, fobj;
 long id = otypes[cbObject->currentItem()].toLong();
 uint n;
 cobj=md->find(id);

 cbForm->clear();
 formslist.clear();

 cbForm->insertItem( QObject::tr("default") );
 formslist.append( "0" );
 if ( md->objClass( cobj ) == md_catalogue )
 {
  cbForm->insertItem( QObject::tr("embedded editor") );
  formslist.append( "1" );
 }
 n = md->count( cobj, md_form );
 for ( uint i = 0; i < n; i++ )
 {
  fobj = md->find( cobj, md_form, i);
  if ( fobj.isNull() )
   continue;
  cbForm->insertItem( md->attr(fobj, mda_name) );
  formslist.append(md->attr(fobj, mda_id));
 }
}
