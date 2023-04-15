#ifndef DELETEQUERYDIALOG_H
#define DELETEQUERYDIALOG_H

#include "querydialog.h"

class DeleteQueryDialog : public QueryDialog{
  Q_OBJECT
public:
  DeleteQueryDialog(bool, QSettings*);

signals:
  void signalAccepted(const QString&);

private slots:
  void m_initilazeContent() override;
  void m_acceptQuery() override;
  void m_cancelQuery() override;
};

#endif // DELETEQUERYDIALOG_H
