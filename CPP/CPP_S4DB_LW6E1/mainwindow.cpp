#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){
    label = new QLabel(this);
    label->setGeometry(0, 25, 100, 100);
    button = new QPushButton("aaa",this);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(cconnect()));
//    if(db.open()){
//      QSqlQuery query;
//      query.exec("SELECT * from call");
//      QSqlRecord rec = query.record();
//      int columnCount = rec.count();

//      while (query.next()) {
//          for (int i = 0; i < columnCount; i++) {
//              QString value = query.value(i).toString();
//              qDebug() << rec.fieldName(i) << ": " << value;
//          }
//      }
//      db.close();
//    }
//    else{
//      label->setText("Error");
//    }

}

void MainWindow::print(){
  label->setText("connected");
}

void MainWindow::cconnect(){
  PostgreConnection& c = PostgreConnection::getInstance();
  connect(&c, SIGNAL(signalConnected()), this, SLOT(print()));
  qDebug() << c.isOpen();
  const bool& ic = c.connectToDatabase("192.168.3.11", "mydb", "postgres", "0437", 5432);
  if(ic){
    qDebug() << "connected";
  }
  else{
    qDebug() << "error";
  }
  qDebug() << c.isOpen() << ic << c.isConnected();
  c.removeDatabase();
  qDebug() << c.isOpen() << ic << c.isConnected();
}

MainWindow::~MainWindow(){
}

