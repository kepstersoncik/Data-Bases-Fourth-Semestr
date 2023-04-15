#include "updatequerydialog.h"

UpdateQueryDialog::UpdateQueryDialog(bool strictMode, QSettings* settings)
  : QueryDialog(strictMode, settings){
  connect(m_tablesCombobox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(m_initilazeContent()));
  m_initilazeContent();

  connect(okButton, SIGNAL(clicked(bool)), this, SLOT(m_acceptQuery()));
  connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(m_cancelQuery()));
}

void UpdateQueryDialog::m_initilazeContent(){
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

  QLabel* attributeLabel = new QLabel("Название атрибута");
  m_entryLayout->addWidget(attributeLabel, 0, 0);
  QLabel* signLabel = new QLabel("Знак");
  m_entryLayout->addWidget(signLabel, 0, 1);
  QLabel* filterLabel = new QLabel("Фильтр");
  m_entryLayout->addWidget(filterLabel, 0, 2);
  QLabel* newLabel = new QLabel("Новое значение");
  m_entryLayout->addWidget(newLabel, 0, 3);
  int row = 1;
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
    QComboBox* comboBox = new QComboBox();
    comboBox->addItem(">");
    comboBox->addItem("<");
    comboBox->addItem("=");
    comboBox->addItem("<>");
    comboBox->addItem(">=");
    comboBox->addItem("<=");
    QLineEdit* filterLineEdit = new QLineEdit();
    QLineEdit* newLineEdit = new QLineEdit();

    m_entryLayout->addWidget(label, row, 0);
    m_entryLayout->addWidget(comboBox, row, 1);
    m_entryLayout->addWidget(filterLineEdit, row, 2);
    m_entryLayout->addWidget(newLineEdit, row, 3);

    row++;
  }
  m_settings->endGroup();
  dbConnection.close();
}

void UpdateQueryDialog::m_acceptQuery(){
  QString queryText = "UPDATE ";
  QString whereText = " WHERE ";
  QString table = m_tablesCombobox->currentData(Qt::ToolTipRole).toString();
  queryText += table + " SET ";
  int numRows = m_entryLayout->rowCount();
  for (int row = 1; row < numRows; row++) {
    QWidget* atributeName = m_entryLayout->itemAtPosition(row, 0)->widget();
    QLineEdit* atributeValue = qobject_cast<QLineEdit*>(\
          m_entryLayout->itemAtPosition(row, 3)->widget()\
    );
    QString value = atributeValue->text().simplified();
    if (atributeName && !value.isEmpty()) {
      queryText += atributeName->toolTip() + "=" + "'" + value + "'";
      if (row != numRows - 1) {
        queryText += ", ";
      }
    }
  }
  for (int row = 1; row < numRows; row++){
    QWidget* atributeName = m_entryLayout->itemAtPosition(row, 0)->widget();
    QLineEdit* atributeValue = qobject_cast<QLineEdit*>(\
          m_entryLayout->itemAtPosition(row, 2)->widget()\
    );
    QComboBox* signComboBox = qobject_cast<QComboBox*>(\
          m_entryLayout->itemAtPosition(row, 1)->widget()\
    );
    QString value = atributeValue->text().simplified();
    QString sign = signComboBox->currentText();
    if (atributeName && !value.isEmpty()){
      whereText += atributeName->toolTip() + sign + "'" + value + "'";
      if (row != numRows - 1){
        whereText += " AND ";
      }
    }
  }
  if (queryText[queryText.length()-2] == ','){
    queryText.chop(2);
  }
  if (whereText.length() >= 4){
    if (whereText[whereText.length()-2] == 'D'){
      whereText.chop(4);
    }
  }
  queryText += whereText;
  qDebug() << queryText;
  emit signalAccepted(queryText);
  accept();
}

void UpdateQueryDialog::m_cancelQuery(){
  reject();
}
