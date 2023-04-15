#include "postgreconnection.h"

PostgreConnection::PostgreConnection(){}
PostgreConnection::~PostgreConnection(){}

bool PostgreConnection::m_isConnected = false;

const int PostgreConnection::m_DEFAULT_PORT = 5432;

QString PostgreConnection::m_connected = "База данных успешно подключена";
QString PostgreConnection::m_connectionError = "Ошибка подключения к базе данных";
QString PostgreConnection::m_disconnected = "База данных отключена";

PostgreConnection& PostgreConnection::getInstance(){
  static PostgreConnection instance;
  return instance;
}



const bool& PostgreConnection::connectToDatabase(const QString& host,
                                                 const QString& database,
                                                 const QString& username,
                                                 const QString& password,
                                                 const int& port){
  if(m_isConnected){
    emit signalConnected(m_connected);
    return m_isConnected;
  }

  m_database = QSqlDatabase::addDatabase("QPSQL", "singleton_postgresql");

  m_database.setHostName(host);
  m_database.setPort(port);
  m_database.setDatabaseName(database);
  m_database.setUserName(username);
  m_database.setPassword(password);

  m_query = QSqlQuery(m_database);

  if (m_database.open()){
    m_isConnected = true;
    m_database.close();
    emit signalConnected(m_connected);
  }
  else{
    m_isConnected = false;
    emit signalError(m_connectionError);
  }
  return m_isConnected;
}

const bool& PostgreConnection::connectToDatabase(ConnectionData &data){
  QString host = data.getHost();
  int port = data.getPort().toInt();
  QString database = data.getDatabase();
  QString username = data.getUsername();
  QString password = data.getPassword();

  return connectToDatabase(host, database, username, password, port);
}



void PostgreConnection::removeDatabase(){
  if(!m_isConnected){
    return;
  }
  m_database.close();
  m_database.removeDatabase("singleton_postgresql");
  m_isConnected = false;
  emit signalDisconnected(m_disconnected);
}



bool PostgreConnection::isOpen(){
  return m_database.isOpen();
}



bool PostgreConnection::open(){
  return m_database.open();
}



void PostgreConnection::close(){
  m_database.close();
}



QSqlQuery& PostgreConnection::query(){
  return m_query;
}

QSqlTableModel* PostgreConnection::tableModel(const QString& table){
  QSqlTableModel* model = new QSqlTableModel(nullptr, m_database);
  model->setTable(table);
  return model;
}

QStringList PostgreConnection::tables(){
  return m_database.tables();
}



const bool& PostgreConnection::isConnected(){
  return m_isConnected;
}
