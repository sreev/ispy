#ifndef IGGI_IGGI_MAIN_WINDOW_H
# define IGGI_IGGI_MAIN_WINDOW_H

# include "ui_IgMainWindow.h"
# include <QObject>

class IgLocationDialog;
class IgSettingsEditor;
class QSettings;
class QMdiArea;

class ISpyMainWindow : public QMainWindow, public Ui::IgMainWindow
{
    Q_OBJECT
public:
    ISpyMainWindow (QWidget *parent = 0);
    ~ISpyMainWindow ();

    void 		restoreSettings (void);    
    void 		saveSettings (void);
    QMdiArea *		workspace (void);    
    
public slots:
    void 		writeSettings (bool value);
    void 		showSettingsEditor (void);    
    void 		about (void);
    void 		maximize (void);
    void 		fullScreen (void);

signals:
    void 		open (void);
    void		autoEvents (void);
    void		nextEvent (void);
    void		previousEvent (void);
    void		rewind (void);
    void		print (void);
    void		save (void);

protected:
    void 		setupActions (void);
    
private:
    IgSettingsEditor   *m_settingsEditor;

    void 		setSettingsObject (QSettings *settings);
};

#endif // IGGI_IGGI_MAIN_WINDOW_H