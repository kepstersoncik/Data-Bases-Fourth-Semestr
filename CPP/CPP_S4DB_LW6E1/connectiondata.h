#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <QSettings>

#include <QHostAddress>

#include <QString>

class ConnectionData{
public:
  ConnectionData(QSettings& settings);
  ConnectionData(const QString& host, const QString& port,
                 const QString& database, const QString& username,
                 const QString& password, bool& ok);

  QString getHost();
  void setHost(const QString& host, bool& ok);

  QString getPort();
  void setPort(const QString& port, bool& ok);

  QString getDatabase();
  void setDatabase(const QString& database, bool& ok);

  QString getUsername();
  void setUsername(const QString& username, bool& ok);

  QString getPassword();
  void setPassword(const QString& username, bool& ok);

  QString validateConnectionData();

protected:
  QString m_host;
  QString m_port;
  QString m_database;
  QString m_username;
  QString m_password;

  bool m_validateHost(const QString& host);
  bool m_validatePort(const QString& port);
};

#endif // CONNECTIONDATA_H
