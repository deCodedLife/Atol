#include "atol.h"

Atol::Atol(QObject *parent) : QObject(parent)
{
    m_tick.SetTime(1000);
    m_tick.configure(m_timerThread);
    m_tick.moveToThread(&m_timerThread);

    m_timerThread.start();
}

void Atol::ChangeConfiguration(Configs *c)
{
    cfg = c;
    jwt.Init(cfg);

    connect(cfg, &Configs::configsLoaded, &api, &Mewbas::UpdateConfig);
    connect(cfg, &Configs::configsLoaded, &core, &Server::UpdateConfig);
    connect(cfg, &Configs::configsLoaded, &jwt, &JWT::SetConfiguration);
    connect(cfg, &Configs::configsLoaded, &reciepts, &Reciepts::UpdateConfig);

    connect(&core, &Server::updateStatus, &api, &Mewbas::ChangeStatus);
    connect(&core, &Server::updateStatus, &reciepts, &Reciepts::HandleReciept);
    connect(&core, &Server::skipped, &api, &Mewbas::NextTask);
    connect(&core, &Server::updateRecieptCode, &api, &Mewbas::ChangeRecieptCode);

    connect(&core, &Server::block, &api, &Mewbas::Block);
    connect(&core, &Server::restore, &api, &Mewbas::Restore);


    connect(&api, &Mewbas::newTask, &core, &Server::GotTask);

#if (AUTO_CONFIRM == false)
    connect(&httpSrv, &HttpServer::recivedRequest, &core, &Server::AtolRecived);
#endif

    connect(&m_tick, &Timer::timeout, &api.RequestDaemon, &Daemon::Countdown);
    connect(&m_tick, &Timer::timeout, &core.TimeoutDaemon, &Daemon::Countdown);
    connect(&m_tick, &Timer::timeout, &core.EpayTimeout, &Daemon::Countdown);

    httpSrv.configure(serverThread);
    httpSrv.moveToThread(&serverThread);
    serverThread.start();

    cfg->Read();
}

void Atol::Ready()
{

}
