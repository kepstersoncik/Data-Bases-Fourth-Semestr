#ifndef POSTGRECONNECTION_H
#define POSTGRECONNECTION_H

#include <QObject>

#include <QString>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <QDebug>


class PostgreConnection : public QObject
{
  Q_OBJECT
public:
  static PostgreConnection& getInstance();

  const bool& connectToDatabase(const QString& host, const QString& name,
                         const QString& user, const QString& password,
                         const int& port = m_DEFAULT_PORT);
  void removeDatabase();

  bool isOpen();

  const bool& isConnected();

  PostgreConnection(PostgreConnection const&) = delete;
  void operator =(PostgreConnection const&) = delete;
signals:
  void signalConnected();
  void signalDisconnected();
  void signalError(const QString&);

protected:
  QSqlDatabase m_database;
  static bool m_isConnected;
  static const int m_DEFAULT_PORT;

protected:
  PostgreConnection();
  ~PostgreConnection();
};

#endif // POSTGRECONNECTION_H
