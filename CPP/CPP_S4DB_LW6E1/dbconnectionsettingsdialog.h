#ifndef DBCONNECTIONSETTINGSDIALOG_H
#define DBCONNECTIONSETTINGSDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QFileDialog>

#include <QSettings>

#include <QStyle>

#include <QString>
#include <QVariant>
#include <QFile>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "connectiondata.h"

#include <QDebug>

class DbConnectionSettingsDialog : public QDialog
{
  Q_OBJECT
public:
  DbConnectionSettingsDialog(QSettings&, QWidget*);
  ~DbConnectionSettingsDialog();

  const QString getHost();
  const QString getPort();
  const QString getDatabase();
  const QString getUsername();
  const QString getPassword();
  const ConnectionData getConnectionData();
protected:
  QVBoxLayout* mainLayout;
  QHBoxLayout* buttonsLayout;
  QGridLayout* dataGridLayout;

  QLabel* serverLabel;
  QLabel* portLabel;
  QLabel* dbNameLabel;
  QLabel* nameLabel;
  QLabel* passwordLabel;

  QLineEdit* serverLineEdit;
  QLineEdit* portLineEdit;
  QLineEdit* dbNameLineEdit;
  QLineEdit* nameLineEdit;
  QLineEdit* passwordLineEdit;

  QPushButton* okButton;
  QPushButton* cancelButton;
  QPushButton* saveButton;
  QPushButton* loadButton;

protected slots:
  void okAndClose();
  void cancelAndClose();
  void loadSettings();
  void saveSettings();
};

#endif // DBCONNECTIONSETTINGSDIALOG_H
