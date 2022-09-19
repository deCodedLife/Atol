#pragma once

#include <QMenu>
#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>

class Tray : public QObject
{
    Q_OBJECT
signals:
    void iconShowed();
    void showed();
    void closed();
    void reload();

public:
    explicit Tray(QObject *parent = nullptr);
    ~Tray();

private slots:
    void Activated(QSystemTrayIcon::ActivationReason);

private:
    QSystemTrayIcon *icon;

};

