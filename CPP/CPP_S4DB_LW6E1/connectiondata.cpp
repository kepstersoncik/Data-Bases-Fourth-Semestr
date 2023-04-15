#include "connectiondata.h"

ConnectionData::ConnectionData(QSettings& settings){
  settings.beginGroup("database");
  m_host = settings.value("host").toString();
  m_port = settings.value("port").toString();
  m_database = settings.value("database").toString();
  m_username = settings.value("username").toString();
  m_password = settings.value("password").toString();
  settings.endGroup();
}

ConnectionData::ConnectionData(const QString &host, const QString &port,
                               const QString &database, const QString &username,
                               const QString &password, bool &ok){
  setHost(host, ok);
  setPort(port, ok);
  setDatabase(database, ok);
  setUsername(username, ok);
  setPassword(password, ok);
}

QString ConnectionData::getHost(){
  return m_host;
}

void ConnectionData::setHost(const QString& host, bool& ok){
  if (!m_validateHost(host)) {
    ok = false;
    return;
  }
  ok = true;
  m_host = host;
}

QString ConnectionData::getPort(){
  return m_port;
}

void ConnectionData::setPort(const QString& port, bool& ok){
  if (!m_validatePort(port)){
    ok = false;
    return;
  }
  ok = true;
  m_port = port;
}

QString ConnectionData::getDatabase(){
  return m_database;
}

void ConnectionData::setDatabase(const QString& database, bool& ok){
  if (database == ""){
    ok = false;
    return;
  }
  ok = true;
  m_database = database;
}

QString ConnectionData::getUsername(){
  return m_username;
}

void ConnectionData::setUsername(const QString& username, bool& ok){
  if (username == ""){
    ok = false;
    return;
  }
  ok = false;
  m_username = username;
}

QString ConnectionData::getPassword(){
  return m_password;
}

void ConnectionData::setPassword(const QString& password, bool& ok){
  if (password == ""){
    ok = false;
    return;
  }
  m_password = password;
}

QString ConnectionData::validateConnectionData(){
  QString errorMessage = "";
  if (!m_validateHost(m_host)) errorMessage += "Ошибка чтения ip адреса сервера\n";
  if (!m_validatePort(m_port)) errorMessage += "Ошибка чтения порта сервера\n";
  if (m_database == "") errorMessage += "Название базы данных не может быть пустым\n";
  if (m_username == "") errorMessage += "Имя пользователя не может быть пустым\n";
  if (m_password == "") errorMessage += "Пароль не может быть пустым";

  return errorMessage;
}

bool ConnectionData::m_validateHost(const QString &host){
  QHostAddress address(host);
  if (address.isNull()){
    return false;
  }
  return true;
}

bool ConnectionData::m_validatePort(const QString &port){
  int portInt;
  bool parsed;
  portInt = port.toInt(&parsed);
  if (!parsed || ((portInt < 1) || (portInt > 65535))){
    return false;
  }
  return true;
}
