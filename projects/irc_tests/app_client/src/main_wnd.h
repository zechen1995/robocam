
#ifndef __MAIN_WND_H_
#define __MAIN_WND_H_

#include <QtGui>
#include "peer_qxmpp.h"
#include "ui_main_wnd.h"

class MainWnd: public QMainWindow
{
	Q_OBJECT
public:
	MainWnd( QWidget * parent = 0 );
	~MainWnd();

    bool eventFilter( QObject * o, QEvent * e );
signals:
    void sigLog( const QString & );
    void sigImageAccepted();
private slots:
    void slotLog( const QString & );
    void slotImageAccepted();
public:
	int print( lua_State * L );
private:
	void init( lua_State * L );
	QIODevice * inFileHandler( const std::string & fileName );
	void accFileHandler( const std::string & fileName, QIODevice * file );
	void log( const std::string & stri );
    void updatePixmap( const QImage & img = QImage() );
    void sceneResizeEvent( QResizeEvent * e );

    Ui_MainWnd  ui;
	PeerQxmpp * m_peer;
	QTime       m_time;
	QStringList m_logList;
	QHash<QPushButton *, quint8> m_motoBtns;
	quint8                       m_motoVal;
    QGraphicsScene      * m_scene;
    QGraphicsPixmapItem * m_image;
    QPixmap               m_pixmap;

	static const std::string CONFIG_FILE;
	static const int         LOG_MAX;
	static const int         MOTO_TIME_MAX; // Due to big ping in the case of fast movements
	                                          // it is necessary to be ablt
	                                          // to set moto time explicitly.
    static const int MOTO1,
                       MOTO2,
                       MOTO3,
                       MOTO4;
    QImage m_img;
public slots:
	void slotSend( const QString & stri );
	void slotImage();
	void slotVoltages();
	void slotLight();
	void slotMotoEn();
    void slotPowerEn();
	void slotMotoDown();
	void slotMotoUp();
	void slotTimeout();
};



#endif



