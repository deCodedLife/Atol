#include "configs.h"

Configuration Configuration::fromJson(QJsonObject data)
{
    Configuration c;

    c.cashboxID = data[PARAM_CASHBOX_ID].toString();
    c.mewbasAddr = data[PARAM_MEWBASS_ADDRESS].toString();
    c.serverAddr = data[PARAM_SERVER_ADDRESS].toString();
    c.serverID = data[PARAM_SERVER_ID].toString();
    c.terminalDir = data[PARAM_TERMINAL_DIR].toString();
    c.jwt = data[PARAM_JWT].toString();
    c.jwtUsername = data[PARAM_JWT_USERNAME].toString();
    c.jwtPassword = data[PARAM_JWT_PASSWORD].toString();
    c.autostart = data[PARAM_AUTOSTART].toBool();
    c.installPath = data[PARAM_INSTALL_PATH].toString();
    c.appExecutable = data[PARAM_APP_EXECUTABLE].toString();

    return c;
}

QJsonObject Configuration::toJson()
{
    QJsonObject configs;

    configs[PARAM_CASHBOX_ID] = cashboxID;
    configs[PARAM_MEWBASS_ADDRESS] = mewbasAddr;
    configs[PARAM_SERVER_ADDRESS] = serverAddr;
    configs[PARAM_SERVER_ID] = serverID;
    configs[PARAM_TERMINAL_DIR] = terminalDir;
    configs[PARAM_JWT] = jwt;
    configs[PARAM_JWT_USERNAME] = jwtUsername;
    configs[PARAM_JWT_PASSWORD] = jwtPassword;
    configs[PARAM_AUTOSTART] = autostart;
    configs[PARAM_INSTALL_PATH] = installPath;
    configs[PARAM_APP_EXECUTABLE] = appExecutable;

    return configs;
}

Configs::Configs() : regestryPath(REGESTRY_PATH), configs(regestryPath, QSettings::NativeFormat)
{
    errorsList.insert(OPENING_FILE_ERROR, {"ERROR", "Configs", "Can't open file", ""});
    errorsList.insert(FILE_IS_EMPTY, {"ERROR", "Configs", "Configuration is empty", ""});
    errorsList.insert(WRONG_CONFIGURATION, {"ERROR", "Configs", "Wrong configuration", ""});
}

void Configs::Read()
{
    QJsonObject configsJson;
    configsJson[PARAM_SERVER_ID] = configs.value(PARAM_SERVER_ID, SERVER_ID).toString();
    configsJson[PARAM_CASHBOX_ID] = configs.value(PARAM_CASHBOX_ID, CASHBOX_ID).toString();
    configsJson[PARAM_MEWBASS_ADDRESS] = configs.value(PARAM_MEWBASS_ADDRESS, MEWBASS_ADDRESS).toString();
    configsJson[PARAM_SERVER_ADDRESS] = configs.value(PARAM_SERVER_ADDRESS, PARAM_SERVER_ADDRESS).toString();
    configsJson[PARAM_TERMINAL_DIR] = configs.value(PARAM_TERMINAL_DIR, TERMINAL_DIR).toString();
    configsJson[PARAM_JWT] = configs.value(PARAM_JWT, "").toString();
    configsJson[PARAM_JWT_USERNAME] = configs.value(JWT_USERNAME, JWT_USERNAME).toString();
    configsJson[PARAM_JWT_PASSWORD] = configs.value(JWT_PASSWORD, JWT_PASSWORD).toString();
    configsJson[PARAM_AUTOSTART] = configs.value(PARAM_AUTOSTART, AUTOSTART).toBool();
    configsJson[PARAM_INSTALL_PATH] = configs.value(PARAM_INSTALL_PATH, QDir().currentPath()).toString();
    configsJson[PARAM_APP_EXECUTABLE] = configs.value(PARAM_APP_EXECUTABLE, "Atol-server2.exe").toString();

    Configuration output = Configuration::fromJson(configsJson);

    lastUpdated = output;
    emit configsLoaded( output );
}

void Configs::write(Configuration newConfiguration)
{
    configs.setValue(PARAM_CASHBOX_ID, newConfiguration.cashboxID);
    configs.setValue(PARAM_SERVER_ID, newConfiguration.serverID);
    configs.setValue(PARAM_MEWBASS_ADDRESS, newConfiguration.mewbasAddr);
    configs.setValue(PARAM_SERVER_ADDRESS, newConfiguration.serverAddr);
    configs.setValue(PARAM_TERMINAL_DIR, newConfiguration.terminalDir);
    configs.setValue(PARAM_JWT, newConfiguration.jwt);
    configs.setValue(PARAM_JWT_USERNAME, newConfiguration.jwtUsername);
    configs.setValue(PARAM_JWT_PASSWORD, newConfiguration.jwtPassword);
    configs.setValue(PARAM_AUTOSTART, newConfiguration.autostart);
    configs.setValue(PARAM_INSTALL_PATH, QDir().currentPath());
    configs.setValue(PARAM_APP_EXECUTABLE, newConfiguration.appExecutable);
}

void Configs::Update(QJsonObject newConfiguration)
{
    if (isValid(newConfiguration) == false)
    {
        Logger::WriteToFile(errorsList[WRONG_CONFIGURATION]);
        return;
    }

    write(Configuration::fromJson(newConfiguration));
}

void Configs::switchAutostart(QJsonObject c)
{
    Configuration configuration = Configuration::fromJson(c);

    QSettings autostartDir(REGESTRY_AUTOSTART, QSettings::NativeFormat);
    QString data = "";

    if (configuration.autostart)
    {
        data = configuration.appExecutable;
    }

    autostartDir.setValue("Atol", data);
}

QJsonObject Configs::QMLGetConfigs()
{
    Read();
    return lastUpdated.toJson();
}

bool Configs::isValid(QJsonObject jsonData)
{
    bool isCorrect = true;

    isCorrect = jsonData.contains(PARAM_CASHBOX_ID) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_MEWBASS_ADDRESS) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_SERVER_ADDRESS) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_SERVER_ID) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_TERMINAL_DIR) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_JWT) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_JWT_USERNAME) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_JWT_PASSWORD) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_AUTOSTART) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_INSTALL_PATH) ? isCorrect : false;
    isCorrect = jsonData.contains(PARAM_APP_EXECUTABLE) ? isCorrect : false;

    return isCorrect;
}

