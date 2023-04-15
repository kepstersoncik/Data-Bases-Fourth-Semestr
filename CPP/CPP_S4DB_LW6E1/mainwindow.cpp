#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString settingsFile)
    : QMainWindow(parent){  
  m_settings = new QSettings(settingsFile, QSettings::IniFormat);
  m_settings->setIniCodec("UTF-8");

  m_settings->beginGroup("presets");
  m_isStrict = m_settings->value("strict_mode").toBool();
  m_settings->endGroup();

  this->setMinimumSize(400, 400);
  m_mainWidget = new QWidget();

  m_mainLayout = new QVBoxLayout();

  m_mainWidget->setLayout(m_mainLayout);
  setCentralWidget(m_mainWidget);

  m_tabelsTabs = new QTabWidget();
  m_tabelsTabs->setGeometry(0, 25, 200, 200);
  m_mainLayout->addWidget(m_tabelsTabs);
  m_tabelsTabs->show();

  m_createActions();
  m_createMenus();
  if (m_isStrict){
    m_initilazeStrictFunctional();
  }
}

MainWindow::~MainWindow(){}

void MainWindow::m_initilazeStrictFunctional(){
  ms_searchQueryAction = new QAction("Поиск по ИНН");
  connect(ms_searchQueryAction, &QAction::triggered,\
          this, &MainWindow::ms_shearchInn);

  ms_procedureQueryAction = new QAction("Процедура вставки");
  connect(ms_procedureQueryAction, &QAction::triggered,\
          this, &MainWindow::ms_procedureQuery);

  ms_pregeneratedQuerysMenu = new QMenu("Запросы");
  ms_pregeneratedQuerysMenu->addAction(ms_searchQueryAction);
  ms_pregeneratedQuerysMenu->addAction(ms_procedureQueryAction);
  menuBar()->addMenu(ms_pregeneratedQuerysMenu);
  ms_pregeneratedQuerysMenu->setDisabled(true);
}

void MainWindow::m_errorHandler(const QString& title, const QString& errorMessage){
  this->setDisabled(true);
  QMessageBox messageBox;
  messageBox.setWindowTitle(title);
  messageBox.setText(errorMessage);
  messageBox.exec();
  this->setDisabled(false);
}


void MainWindow::m_createActions(){
  QStyle* style = QApplication::style();

  m_strictModeAction = new QAction("Строгий режим");
  m_strictModeAction->setCheckable(true);
  m_strictModeAction->setChecked(m_isStrict);
  connect(m_strictModeAction, SIGNAL(toggled(bool)),
          this, SLOT(m_setStrictMode(bool)));

  m_connectToDatabaseAction = new QAction(\
        style->standardIcon(QStyle::SP_MediaPlay), ""\
  );
  connect(m_connectToDatabaseAction, &QAction::triggered,
          this, &MainWindow::m_connectDatabase);
  menuBar()->addAction(m_connectToDatabaseAction);

  m_disconnectDatabaseAction = new QAction(\
        style->standardIcon(QStyle::SP_MediaStop), ""\
  );
  connect(m_disconnectDatabaseAction, &QAction::triggered,
          this, &MainWindow::m_disconnectDatabase);
  menuBar()->addAction(m_disconnectDatabaseAction);
  m_disconnectDatabaseAction->setDisabled(true);

  m_connectionSettingsAction = new QAction("Настройки подключения");
  connect(m_connectionSettingsAction, &QAction::triggered,
          this, &MainWindow::m_connectionSettingsDialog);


  m_insertToTableAction = new QAction("Вставка в таблицу");
  connect(m_insertToTableAction, &QAction::triggered,
          this, &MainWindow::m_insertToTableDialog);

  m_updateTableAction = new QAction("Обновление талицы");
  connect(m_updateTableAction, &QAction::triggered,
          this, &MainWindow::m_updateTableDialog);

  m_deleteInTableAction = new QAction("Удаление из таблицы");
  connect(m_deleteInTableAction, &QAction::triggered,
          this, &MainWindow::m_deleteTableDialog);
}

void MainWindow::m_createMenus(){
  m_programmMenu = menuBar()->addMenu("Программа");
  m_programmMenu->addAction(m_connectionSettingsAction);
  m_programmMenu->addAction(m_strictModeAction);

  m_databaseMenu = menuBar()->addMenu("База данных");
  m_databaseMenu->setDisabled(true);
  m_databaseMenu->addAction(m_insertToTableAction);
  m_databaseMenu->addAction(m_updateTableAction);
  m_databaseMenu->addAction(m_deleteInTableAction);
}


PostgreConnection& MainWindow::m_newConnection()
{
  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  connect(&dbConnection, SIGNAL(signalConnected(QString)),
          statusBar(), SLOT(showMessage(QString)));
  connect(&dbConnection, SIGNAL(signalConnected(QString)),
          statusBar(), SLOT(showMessage(QString)));
  return dbConnection;
}

void MainWindow::m_connectionErrorHandler(const QString &errorMessage){
  m_errorHandler("Ошибка подключения", errorMessage);
}

void MainWindow::m_showTables(PostgreConnection& dbConnection)
{
  m_tabelsTabs->clear();

  if(!dbConnection.open()){
    m_connectionErrorHandler("Не удалось получить соединение с базой данных");
    return;
  }

  QStringList tables = dbConnection.tables();
  for (QString table : tables) {
    QSqlTableModel* model = dbConnection.tableModel(table);
    if (!model->select()) {
      m_connectionErrorHandler("Не удалось получить доступ к таблице " + table);
      continue;
    }
    if (m_isStrict){
      for (int i = 0; i < model->columnCount(); i++) {
        QVariant header = model->headerData(i, Qt::Horizontal, Qt::DisplayRole);
        m_settings->beginGroup("strict_column_names");
        QString strictName = m_settings->value(header.toString()).toString();
        m_settings->endGroup();
        model->setHeaderData(i, Qt::Horizontal, strictName);
      }
    }

    QTableView* view = m_createTableView(model);
    if (m_isStrict){
      m_settings->beginGroup(m_isStrict ? "strict_table_names" : "");
      m_tabelsTabs->addTab(view, m_settings->value(table).toString());
      m_settings->endGroup();
    }
    else{
      m_tabelsTabs->addTab(view, table);
    }
  }

  m_tabelsTabs->show();
  dbConnection.close();
}

QTableView *MainWindow::m_createTableView(QSqlTableModel *model){
  QTableView* view = new QTableView();
  view->setModel(model);
  return view;
}

void MainWindow::m_queryErrorHandler(const QString& errorMessage){
  m_errorHandler("Ошибка выполнения запроса", errorMessage);
}

bool MainWindow::m_isCompatibleWithStrictMode(PostgreConnection& dbConnection){
  m_settings->beginGroup("strict_table_names");
  QStringList strictTableNames = m_settings->allKeys();
  qDebug() << strictTableNames;
  m_settings->endGroup();

  dbConnection.open();
  QStringList tables = dbConnection.tables();
  qDebug() << tables;
  dbConnection.close();

  std::sort(strictTableNames.begin(), strictTableNames.end());
  std::sort(tables.begin(), tables.end());
  return tables == strictTableNames;
}

void MainWindow::m_setStrictMode(const bool& mode){
  m_settings->beginGroup("presets");
  m_settings->setValue("strict_mode", mode);
  m_settings->endGroup();
  m_isStrict=mode;
  if (m_isStrict){
    m_initilazeStrictFunctional();
  }
  else {
    menuBar()->clear();
    m_createActions();
    m_createMenus();
  }
}

void MainWindow::m_connectDatabase(){
  PostgreConnection& dbConnection = m_newConnection();

  ConnectionData data(*m_settings);

  QString error = data.validateConnectionData();
  if(error != ""){
    m_connectionErrorHandler(error);
    return;
  }

  const bool& ic = dbConnection.connectToDatabase(data);

  if(!ic){
    m_connectionErrorHandler("Не удалось получить соединение с базой данных");
    return;
  }

  if (m_isStrict){
    bool compatibleWithStrict = m_isCompatibleWithStrictMode(dbConnection);
    if (!compatibleWithStrict){
      m_connectionErrorHandler("База данных не совместима со строгим режимом");
      dbConnection.removeDatabase();
      return;
    }
  }

  m_showTables(dbConnection);
  m_databaseMenu->setDisabled(false);
  m_disconnectDatabaseAction->setDisabled(false);
  ms_pregeneratedQuerysMenu->setDisabled(false);
  m_connectToDatabaseAction->setDisabled(true);
  m_strictModeAction->setDisabled(true);
}

void MainWindow::m_disconnectDatabase(){
  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  if (dbConnection.isOpen()){
    dbConnection.close();
  }
  if (dbConnection.isConnected()){
    dbConnection.removeDatabase();
  }
  m_tabelsTabs->clear();
  m_databaseMenu->setDisabled(true);
  m_disconnectDatabaseAction->setDisabled(true);
  ms_pregeneratedQuerysMenu->setDisabled(true);
  m_connectToDatabaseAction->setDisabled(false);
  m_strictModeAction->setDisabled(false);
}

void MainWindow::m_connectionSettingsDialog(){
  DbConnectionSettingsDialog* dialog = new DbConnectionSettingsDialog(*m_settings,
                                                                      this);
  dialog->exec();
  int result = dialog->result();
  if (result == QDialog::Rejected){
    delete dialog;
    return;
  }

  ConnectionData dialogConnectionData = dialog->getConnectionData();

  m_settings->beginGroup("database");
  m_settings->setValue("host", dialogConnectionData.getHost());
  m_settings->setValue("port", dialogConnectionData.getPort());
  m_settings->setValue("database", dialogConnectionData.getDatabase());
  m_settings->setValue("username", dialogConnectionData.getUsername());
  m_settings->setValue("password", dialogConnectionData.getPassword());
  m_settings->endGroup();
  delete dialog;
}

void MainWindow::ms_shearchInn(){
  QString inn = QInputDialog::getText(nullptr, "Введите ИНН", "ИНН:");
  inn = inn.simplified();
  if (inn.isEmpty()){
    m_queryErrorHandler("Не удалось выполнить запрос так как строка пустая");
    return;
  }
  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  m_showTables(dbConnection);
  if(!dbConnection.open()){
    m_connectionErrorHandler("Не удалось получить соединение с базой данных");
    return;
  }
  QSqlQuery query = dbConnection.query();
  query.prepare("SELECT * FROM call WHERE inn = ?");
  query.addBindValue(inn);
  QSqlQueryModel* model = new QSqlQueryModel();
  if (query.exec()){
    model->setQuery(query);
  }
  else{
    m_queryErrorHandler("Не удалось выполнить запрос\n" + query.lastError().text());
    dbConnection.close();
    return;
  }
  QTableView* view = new QTableView();
  view->setModel(model);
  m_tabelsTabs->addTab(view, "Результаты поиска");
  dbConnection.close();
}

void MainWindow::ms_procedureQuery(){
  QStringList labels = {"Телефонная точка", "ИНН", "Рассчетный счет",\
                        "Время суток", "Длительность", "Дата звонка",\
                        "Город", "Скидка", "Дневной тариф", "Ночной тариф"};

  QFormLayout* layout = new QFormLayout();
  QList<QLineEdit*> lineEdits;
  for (int i = 0; i < labels.size(); ++i) {
    QLineEdit* lineEdit = new QLineEdit();
    lineEdits.append(lineEdit);
    layout->addRow(labels.at(i), lineEdit);
  }

  QDialog dialog;
  dialog.setLayout(layout);
  dialog.setModal(true);
  QPushButton* acceptButton = new QPushButton("Применить");
  connect(acceptButton, &QPushButton::clicked, &dialog, &QDialog::accept);
  layout->addWidget(acceptButton);

  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  if(!dbConnection.open()){
    m_connectionErrorHandler("Не удалось получить соединение с базой данных");
    return;
  }
  QSqlQuery query = dbConnection.query();
  if (dialog.exec() == QDialog::Accepted) {
    QStringList values;
    for (QLineEdit* lineEdit : lineEdits){
      values.append("'" + lineEdit->text() + "'");
    }
    bool ok = query.exec("CALL insert_call(" + values.join(", ") + ")");
    if (!ok){
      m_queryErrorHandler("Не удалось выполнить запрос\n" + query.lastError().text());
      dbConnection.close();
      return;
    }
  }
  dbConnection.close();
  m_showTables(dbConnection);

  qDeleteAll(lineEdits);
  delete layout;
}

void MainWindow::m_insertToTableDialog(){
  dialog = new InsertQueryDialog(m_isStrict, m_settings);
  connect(dialog, SIGNAL(signalAccepted(QString)),\
          this, SLOT(m_executeDialogQuery(QString)));
  dialog->exec();
  delete dialog;
}

void MainWindow::m_executeDialogQuery(const QString& query){
  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  dbConnection.open();
  QSqlQuery& dbQuery = dbConnection.query();

  if (!dbQuery.exec(query)){
    m_queryErrorHandler(dbQuery.lastError().text());
  }
  dbConnection.close();
  m_showTables(dbConnection);
}

void MainWindow::m_updateTableDialog(){
  dialog = new UpdateQueryDialog(m_isStrict, m_settings);
  connect(dialog, SIGNAL(signalAccepted(QString)), this, SLOT(m_executeDialogQuery(QString)));
  dialog->exec();
  delete dialog;
}

void MainWindow::m_deleteTableDialog(){
  dialog = new DeleteQueryDialog(m_isStrict, m_settings);
  connect(dialog, SIGNAL(signalAccepted(QString)), this, SLOT(m_executeDialogQuery(QString)));
  dialog->exec();
  delete dialog;
}



