#include "dbconnectionsettingsdialog.h"

DbConnectionSettingsDialog::DbConnectionSettingsDialog(QSettings& settings,
                                                       QWidget *parent = nullptr)
  :QDialog(parent){
  this->setFixedSize(250, 200);

  mainLayout = new QVBoxLayout();
  buttonsLayout = new QHBoxLayout();
  dataGridLayout = new QGridLayout();

  mainLayout->addLayout(dataGridLayout);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);

  serverLabel = new QLabel("Сервер");
  portLabel = new QLabel("Порт");
  dbNameLabel = new QLabel("База данных");
  nameLabel = new QLabel("Имя пользователя");
  passwordLabel = new QLabel("Пароль");

  settings.beginGroup("database");
  serverLineEdit = new QLineEdit(settings.value("host").toString());
  portLineEdit = new QLineEdit(settings.value("port").toString());
  dbNameLineEdit = new QLineEdit(settings.value("database").toString());
  nameLineEdit = new QLineEdit(settings.value("username").toString());
  passwordLineEdit = new QLineEdit(settings.value("password").toString());
  settings.endGroup();

  dataGridLayout->addWidget(serverLabel, 0, 0);
  dataGridLayout->addWidget(serverLineEdit, 0, 1);

  dataGridLayout->addWidget(portLabel, 1, 0);
  dataGridLayout->addWidget(portLineEdit, 1, 1);

  dataGridLayout->addWidget(dbNameLabel, 2, 0);
  dataGridLayout->addWidget(dbNameLineEdit, 2, 1);

  dataGridLayout->addWidget(nameLabel, 3, 0);
  dataGridLayout->addWidget(nameLineEdit, 3, 1);

  dataGridLayout->addWidget(passwordLabel, 4, 0);
  dataGridLayout->addWidget(passwordLineEdit, 4, 1);


  QStyle* style = QApplication::style();

  okButton = new QPushButton("Принять");
  connect(okButton, SIGNAL(clicked(bool)), this, SLOT(okAndClose()));

  cancelButton = new QPushButton("Отмена");
  connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(cancelAndClose()));

  saveButton = new QPushButton();
  connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
  saveButton->setIcon(style->standardIcon(QStyle::SP_DialogSaveButton));

  loadButton = new QPushButton();
  connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(loadSettings()));
  loadButton->setIcon(style->standardIcon(QStyle::SP_DialogOpenButton));

  buttonsLayout->addWidget(okButton);
  buttonsLayout->addWidget(cancelButton);
  buttonsLayout->addWidget(saveButton);
  buttonsLayout->addWidget(loadButton);
}

DbConnectionSettingsDialog::~DbConnectionSettingsDialog(){
  delete serverLabel;
  delete portLabel;
  delete dbNameLabel;
  delete nameLabel;
  delete passwordLabel;

  delete serverLineEdit;
  delete portLineEdit;
  delete dbNameLineEdit;
  delete nameLineEdit;
  delete passwordLineEdit;

  delete okButton;
  delete cancelButton;
  delete saveButton;
  delete loadButton;

  delete buttonsLayout;
  delete dataGridLayout;
  delete mainLayout;
}



const QString DbConnectionSettingsDialog::getHost(){
  return serverLineEdit->text();
}

const QString DbConnectionSettingsDialog::getPort(){
  return portLineEdit->text();
}

const QString DbConnectionSettingsDialog::getDatabase(){
  return dbNameLineEdit->text();
}

const QString DbConnectionSettingsDialog::getUsername(){
  return nameLineEdit->text();
}

const QString DbConnectionSettingsDialog::getPassword(){
  return passwordLineEdit->text();
}

const ConnectionData DbConnectionSettingsDialog::getConnectionData(){
  QString host = getHost();
  QString port = getPort();
  QString database = getDatabase();
  QString username = getUsername();
  QString password = getPassword();

  bool ok = false;
  ConnectionData connectionData(host, port, database, username, password, ok);
  return connectionData;
}

void DbConnectionSettingsDialog::okAndClose(){
  accept();
}

void DbConnectionSettingsDialog::cancelAndClose(){
  reject();
}

void DbConnectionSettingsDialog::loadSettings(){
  QFileDialog dialog(this);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("Text files (*.json)"));

  QString fileName = tr("config.json");
  fileName = dialog.getOpenFileName(this, tr("Открыть файл"),
                                    QDir::currentPath(), tr("Text files (*.json)"));


  QFile configFile(fileName);
  if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
    qDebug() << "err";
    return;
  }

  QJsonParseError jsonError;
  QJsonDocument configDoc = QJsonDocument::fromJson(configFile.readAll(),
                                                    &jsonError);
  if (jsonError.error != QJsonParseError::NoError){
    qDebug() << "pars err";
    return;
  }

  QString server = configDoc.object().value("host").toString();
  QString port = configDoc.object().value("port").toString();
  QString database = configDoc.object().value("database").toString();
  QString username = configDoc.object().value("username").toString();
  QString password = configDoc.object().value("password").toString();

  serverLineEdit->setText(server);
  portLineEdit->setText(port);
  dbNameLineEdit->setText(database);
  nameLineEdit->setText(username);
  passwordLineEdit->setText(password);
}

void DbConnectionSettingsDialog::saveSettings(){
  QJsonObject json;
  json["host"] = serverLineEdit->text();
  json["port"] = portLineEdit->text();
  json["database"] = dbNameLineEdit->text();
  json["username"] = nameLineEdit->text();
  json["password"] = passwordLineEdit->text();

  QJsonDocument jsonDoc(json);

  QString dirName = QFileDialog::getExistingDirectory(nullptr, "Сохранить файл", "");
  if (dirName.isEmpty()){
    return;
  }

  QFile file(dirName + "/config.json");
  if (!file.open(QIODevice::WriteOnly)){
    return;
  }

  file.write(jsonDoc.toJson());
  file.close();
}
