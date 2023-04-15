#include "querydialog.h"

QueryDialog::QueryDialog(bool strictMode, QSettings* settings, QWidget* parent)
  :QDialog(parent){
  m_isStrictMode = strictMode;

  m_settings = settings;

  m_mainLayout = new QVBoxLayout();

  m_tablesCombobox = new QComboBox();
  m_addTablesToCombobox();
  m_mainLayout->addWidget(m_tablesCombobox);

  m_entryLayout = new QGridLayout();
  m_mainLayout->addLayout(m_entryLayout);

  m_buttonsLayout = new QHBoxLayout();
  m_mainLayout->addLayout(m_buttonsLayout);

  okButton = new QPushButton("Принять");
  m_buttonsLayout->addWidget(okButton);

  cancelButton = new QPushButton("Отмена");
  m_buttonsLayout->addWidget(cancelButton);

  setLayout(m_mainLayout);
}

QueryDialog::~QueryDialog(){
  delete m_tablesCombobox;

  delete m_entryLayout;
  delete m_buttonsLayout;
  delete m_mainLayout;
}

void QueryDialog::m_clearEntryLayout(){
  int numRows = m_entryLayout->rowCount();
  int numColumns = m_entryLayout->columnCount();

  for (int row = 0; row < numRows; row++){
    for (int column = 0; column < numColumns; column++){
      QLayoutItem* item = m_entryLayout->itemAtPosition(row, column);
      if (item != nullptr){
        m_entryLayout->removeItem(item);
        delete item->widget();
        delete item;
      }
    }
  }
}

void QueryDialog::m_addTablesToCombobox(){
  m_tablesCombobox->clear();
  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  if (!dbConnection.open()){
    QMessageBox messageBox;
    messageBox.setWindowTitle("Ошибка подключения");
    messageBox.setText("Не удалось установить соединение с базой данных");
    messageBox.exec();
    return;
  }
  QStringList tables = dbConnection.tables();
  QStandardItemModel* model = m_createTableModel(tables);
  m_tablesCombobox->setModel(model);
  dbConnection.close();
}

QStandardItemModel* QueryDialog::m_createTableModel(const QStringList& tables){
  QStandardItemModel* model = new QStandardItemModel();
  for (QString table : tables){
    QStandardItem* item;
    if (m_isStrictMode) {
      m_settings->beginGroup("strict_table_names");
      item = new QStandardItem(m_settings->value(table).toString());
      m_settings->endGroup();
    } else {
      item = new QStandardItem(table);
    }
    item->setEditable(false);
    item->setToolTip(table);
    model->appendRow(item);
  }
  return model;
}
