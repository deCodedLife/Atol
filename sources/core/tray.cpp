#include "tray.h"

Tray::Tray(QObject *parent) : QObject(parent)
{
    QMenu *trayMenu = new QMenu();

    QAction *showAction = new QAction("Развернуть", this);
    QAction *quitAction = new QAction("Выйти", this);
    QAction *reloadAction = new QAction("Перезагрузить", this);

    connect(showAction, &QAction::triggered, this, &Tray::showed);
    connect(reloadAction, &QAction::triggered, this, &Tray::reload);
    connect(quitAction, &QAction::triggered, this, &Tray::closed);

    trayMenu->addAction(showAction);
    trayMenu->addAction(reloadAction);
    trayMenu->addAction(quitAction);

    icon = new QSystemTrayIcon();
    icon->setContextMenu(trayMenu);
    icon->setIcon(QIcon(":/images/appIcon.ico"));
    icon->show();

    connect(icon, &QSystemTrayIcon::activated, this, &Tray::Activated);
}

Tray::~Tray()
{
    icon->hide();
}

void Tray::Activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason != QSystemTrayIcon::Trigger)
    {
        return;
    }

    emit iconShowed();
}
