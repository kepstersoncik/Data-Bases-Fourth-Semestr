#ifndef POSTGRECONNECTION_H
#define POSTGRECONNECTION_H

#include <QObject>

#include <QString>
#include <QStringList>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

#include "connectiondata.h"

#include <QDebug>


class PostgreConnection : public QObject
{
  Q_OBJECT
public:
  static PostgreConnection& getInstance();

  const bool& connectToDatabase(const QString& host, const QString& name,
                         const QString& user, const QString& password,
                         const int& port = m_DEFAULT_PORT);
  const bool& connectToDatabase(ConnectionData& data);
  void removeDatabase();

  bool isOpen();

  bool open();
  void close();

  QSqlQuery& query();
  QSqlTableModel* tableModel(const QString& table);
  QStringList tables();

  const bool& isConnected();

  PostgreConnection(PostgreConnection const&) = delete;
  void operator =(PostgreConnection const&) = delete;


signals:
  void signalConnected(const QString&);
  void signalDisconnected(const QString&);
  void signalError(const QString&);

protected:
  QSqlDatabase m_database;
  QSqlQuery m_query;

  static bool m_isConnected;
  static const int m_DEFAULT_PORT;


protected:
  // Сообщения сигналов
  static QString m_connected;
  static QString m_connectionError;
  static QString m_disconnected;

protected:
  PostgreConnection();
  ~PostgreConnection();
};

#endif // POSTGRECONNECTION_H
