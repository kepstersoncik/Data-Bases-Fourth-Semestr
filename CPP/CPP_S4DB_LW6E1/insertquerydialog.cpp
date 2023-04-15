#include "insertquerydialog.h"

InsertQueryDialog::InsertQueryDialog(bool strictMode, QSettings* settings)
  : QueryDialog(strictMode, settings){
  connect(m_tablesCombobox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(m_initilazeContent()));
  m_initilazeContent();

  connect(okButton, SIGNAL(clicked(bool)), this, SLOT(m_acceptQuery()));
  connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(m_cancelQuery()));
}

void InsertQueryDialog::m_initilazeContent(){
  m_clearEntryLayout();

  PostgreConnection& dbConnection = PostgreConnection::getInstance();
  if (!dbConnection.open()){
    this->setDisabled(true);
    QMessageBox messageBox;
    messageBox.setWindowTitle("Ошибка подключения");
    messageBox.setText("Не удалось установить соединение с базой данных");
    messageBox.exec();
    this->setDisabled(false);
    return;
  }
  QString table = m_tablesCombobox->currentData(Qt::ToolTipRole).toString();
  QSqlQuery query = dbConnection.query();
  query.exec("SELECT column_name FROM information_schema.columns"
             " WHERE table_name = '" + table + "'");

  int row = 0;
  m_settings->beginGroup("strict_column_names");
  while (query.next()) {
    QString attribute = query.value(0).toString();
    QString name;
    if (m_isStrictMode){
      name = m_settings->value(attribute).toString();
    }
    else{
      name = attribute;
    }
    QLabel* label = new QLabel(name);
    label->setToolTip(attribute);
    QLineEdit* lineEdit = new QLineEdit();

    m_entryLayout->addWidget(label, row, 0);
    m_entryLayout->addWidget(lineEdit, row, 1);

    row++;
  }
  m_settings->endGroup();
  dbConnection.close();
}

void InsertQueryDialog::m_acceptQuery(){
  QString queryText = "INSERT INTO ";
  QString valuesText = "(";
  QString table = m_tablesCombobox->currentData(Qt::ToolTipRole).toString();
  queryText += table + " (";
  int numRows = m_entryLayout->rowCount();
  for (int row = 0; row < numRows; row++) {
    QWidget* atributeName = m_entryLayout->itemAtPosition(row, 0)->widget();
    QLineEdit* atributeValue = qobject_cast<QLineEdit*>(\
          m_entryLayout->itemAtPosition(row, 1)->widget()\
    );
    QString value = atributeValue->text().simplified();
    if (atributeName && !value.isEmpty()) {
      queryText += atributeName->toolTip();
      valuesText += ("'" + value + "'");
      if (row != numRows - 1) {
        queryText += ", ";
        valuesText += ", ";
      }
    }
  }
  if (queryText[queryText.length()-2] == ','){
    queryText.chop(2);
  }
  if (valuesText.length() >= 2){
    if (valuesText[valuesText.length()-2] == ','){
      valuesText.chop(2);
    }
  }
  queryText += ") VALUES " + valuesText + ")";
  emit signalAccepted(queryText);
}

void InsertQueryDialog::m_cancelQuery(){
  reject();
}
