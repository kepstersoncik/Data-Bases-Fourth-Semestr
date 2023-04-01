#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QSqlDatabase>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QLabel>
#include <QPushButton>

#include <QString>
#include <QDebug>

#include "postgreconnection.h"

class MainWindow : public QMainWindow{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *label;
    QPushButton *button;

  public slots:
    void print();
    void cconnect();
};
#endif // MAINWINDOW_H
