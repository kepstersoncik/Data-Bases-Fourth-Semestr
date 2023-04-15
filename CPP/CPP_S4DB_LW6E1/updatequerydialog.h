#ifndef UPDATEQUERYDIALOG_H
#define UPDATEQUERYDIALOG_H

#include <QComboBox>

#include "querydialog.h"

class UpdateQueryDialog : public QueryDialog{
  Q_OBJECT
public:
  UpdateQueryDialog(bool, QSettings*);

signals:
  void signalAccepted(const QString&);

private slots:
  void m_initilazeContent() override;
  void m_acceptQuery() override;
  void m_cancelQuery() override;
};

#endif // UPDATEQUERYDIALOG_H
