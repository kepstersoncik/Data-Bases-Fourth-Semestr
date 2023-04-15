#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QWidget>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

#include <QSettings>

#include <QStyle>

#include <QHostAddress>
#include <QAbstractSocket>
#include <QString>
#include <QStringList>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QTabWidget>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

#include <QLabel>
#include <QPushButton>
#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QTableView>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QCheckBox>

#include "postgreconnection.h"
#include "dbconnectionsettingsdialog.h"
#include "connectiondata.h"
#include "querydialog.h"
#include "insertquerydialog.h"
#include "updatequerydialog.h"
#include "deletequerydialog.h"


#include <QDebug>

class MainWindow : public QMainWindow{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr, QString settingsFile = "settings.ini");
  ~MainWindow();

protected:
  bool m_isStrict;

  QueryDialog* dialog;

  QSettings* m_settings;

  QWidget* m_mainWidget;
  QVBoxLayout* m_mainLayout;

  QTabWidget* m_tabelsTabs;

  QAction* m_connectToDatabaseAction;
  QAction* m_disconnectDatabaseAction;

  QMenu* m_programmMenu;
  QAction* m_connectionSettingsAction;
  QAction* m_strictModeAction;

  QMenu* m_databaseMenu;
  QAction* m_insertToTableAction;
  QAction* m_updateTableAction;
  QAction* m_deleteInTableAction;

  QMenu* ms_pregeneratedQuerysMenu;
  QAction* ms_searchQueryAction;
  QAction* ms_procedureQueryAction;

  void m_initilazeStrictFunctional();

  void m_errorHandler(const QString&, const QString&);

  void m_createActions();
  void m_createMenus();

  PostgreConnection& m_newConnection();
  void m_connectionErrorHandler(const QString&);

  void m_showTables(PostgreConnection&);
  QTableView* m_createTableView(QSqlTableModel*);

  void m_queryErrorHandler(const QString&);

  bool m_isCompatibleWithStrictMode(PostgreConnection&);

protected slots:
  void m_setStrictMode(const bool&);

  void m_connectDatabase();
  void m_disconnectDatabase();

  void m_connectionSettingsDialog();

  void ms_shearchInn();
  void ms_procedureQuery();

  void m_insertToTableDialog();
  void m_executeDialogQuery(const QString&);

  void m_updateTableDialog();

  void m_deleteTableDialog();
};
#endif // MAINWINDOW_H
