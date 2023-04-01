#include "postgreconnection.h"

PostgreConnection::PostgreConnection(){}
PostgreConnection::~PostgreConnection(){}

const int PostgreConnection::m_DEFAULT_PORT = 5432;
bool PostgreConnection::m_isConnected = false;

PostgreConnection& PostgreConnection::getInstance(){
  static PostgreConnection instance;
  return instance;
}



const bool& PostgreConnection::connectToDatabase(const QString& host, const QString& name,
                                          const QString& user, const QString& password,
                                          const int& port){
  if(m_isConnected){
    emit signalConnected();
    return m_isConnected;
  }

  m_database = QSqlDatabase::addDatabase("QPSQL", "singleton_postgresql");

  m_database.setHostName(host);
  m_database.setPort(port);
  m_database.setDatabaseName(name);
  m_database.setUserName(user);
  m_database.setPassword(password);

  if (m_database.open()){
    m_isConnected = true;
    m_database.close();
    emit signalConnected();
  }
  else{
    m_isConnected = false;
    emit signalError(m_database.lastError().text());
  }
  return m_isConnected;
}



void PostgreConnection::removeDatabase(){
  if(!m_isConnected){
    return;
  }
  m_database.close();
  m_database.removeDatabase("singleton_postgresql");
  m_isConnected = false;
  emit signalDisconnected();
}

bool PostgreConnection::isOpen(){
  return m_database.isOpen();
}

const bool& PostgreConnection::isConnected(){
  return m_isConnected;
}
