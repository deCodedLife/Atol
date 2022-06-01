#include "jwt.h"

JWT::JWT(QObject *parent) : QObject(parent)
{
    errorsList.insert(EMPTY_RESPONCE, {"ERROR", "JWT", "Got empty responce", ""});
    errorsList.insert(WRONG_DATA, {"ERROR", "JWT", "Wrong jwt auth data", ""});
    errorsList.insert(WRONG_SERVER, {"ERROR", "JWT", "Server sen't strange data", "Maybe server url is wrong"});

    connect(&net, &Network::dataRecived, this, &JWT::ReciveJWT);
}

void JWT::Init(Configs *c)
{
    configs = c;
}

void JWT::ReciveJWT(QString reply)
{
    QJsonObject jsonReply = QJsonDocument::fromJson(reply.toUtf8()).object();

    if (jsonReply.isEmpty())
    {
        Logger::WriteToFile(errorsList[EMPTY_RESPONCE]);
        return;
    }

    if (jsonReply.contains("status") == false)
    {
        Logger::WriteToFile(errorsList[WRONG_DATA]);
        return;
    }

    if (jsonReply["status"].toInt() != 200)
    {
        Logger::WriteToFile(errorsList[WRONG_DATA]);
        return;
    }

    currentConfiguration.jwt = jsonReply["data"].toString();

    configs->Update(currentConfiguration.toJson());
    configs->Read();
}

void JWT::SetConfiguration(Configuration c)
{
    if (c.jwt.trimmed().isEmpty() == false)
    {
        return;
    }

    currentConfiguration = c;

    Request signInRequest = Request::GetJWT(c.jwtUsername, c.jwtPassword);
    net.Post(c.mewbasAddr, signInRequest.toJson());
}
