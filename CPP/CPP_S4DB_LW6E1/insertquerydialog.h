#ifndef INSERTQUERYDIALOG_H
#define INSERTQUERYDIALOG_H

#include <QWidget>
#include <QMessageBox>

#include <QAbstractItemView>
#include <QStandardItem>

#include <QString>

#include <QLayoutItem>

#include <QLabel>
#include <QLineEdit>

#include "querydialog.h"
#include "postgreconnection.h"

#include <QDebug>


class InsertQueryDialog : public QueryDialog {
  Q_OBJECT
public:
  InsertQueryDialog(bool, QSettings*);

signals:
  void signalAccepted(const QString&);

private slots:
  void m_initilazeContent() override;
  void m_acceptQuery() override;
  void m_cancelQuery() override;
};

#endif // INSERTQUERYDIALOG_H
