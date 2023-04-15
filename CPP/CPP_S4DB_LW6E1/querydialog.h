#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include <QSettings>

#include <QString>
#include <QStringList>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>

#include <QLayoutItem>
#include <QStandardItemModel>
#include <QStandardItem>

//#include <QSql/

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "postgreconnection.h"

#include <QDebug>

class QueryDialog : public QDialog
{
  Q_OBJECT
public:
  QueryDialog(bool strictMode, QSettings* m_settings, QWidget* parent = nullptr);
  ~QueryDialog();

protected:
  bool m_isStrictMode;

  QSettings* m_settings;

  static const int CITY_MULTIPLIERS =0;
  static const int PRICE = 1;
  static const int CALL = 2;
  static const int SUBSCRIBER = 3;
  static const int SALE = 4;


  QVBoxLayout* m_mainLayout;
  QGridLayout* m_entryLayout;
  void m_clearEntryLayout();
  QHBoxLayout* m_buttonsLayout;


  QComboBox* m_tablesCombobox;
  void m_addTablesToCombobox();
  QStandardItemModel* m_createTableModel(const QStringList&);


  QPushButton* okButton;
  QPushButton* cancelButton;


  QLabel* cityLabel;
  QLineEdit* cityLineEdit;

  QLabel* timesOfDayLabel;
  QLineEdit* timesOfDayLineEdit;

  QLabel* durationLabel;
  QLineEdit* durationLineEdit;

  QLabel* innLabel;
  QLineEdit* innLineEdit;


  QLabel* discountLabel;
  QLineEdit* discountLineEdit;

  QLabel* priceDaytimeLabel;
  QLineEdit* priceDaytimeLineEdit;

  QLabel* priceNightLabel;
  QLineEdit* priceNightLineEdit;


  QLabel* priceLabel;
  QLineEdit* priceLineEdit;


  QLabel* callIdLabel;
  QLineEdit* callIdLineEdit;

  QLabel* callDateLabel;
  QLineEdit* callDateLineEdit;


  QLabel* telephonePointLabel;
  QLineEdit* telephonePointLineEdit;

  QLabel* checkingAccauntLabel;
  QLineEdit* checkingAccauntLineEdit;


  QLabel* saleLabel;
  QLineEdit* saleLineEdit;

protected slots:
  virtual void m_initilazeContent() = 0;
  virtual void m_acceptQuery() = 0;
  virtual void m_cancelQuery() = 0;
};

#endif // QUERYDIALOG_H
