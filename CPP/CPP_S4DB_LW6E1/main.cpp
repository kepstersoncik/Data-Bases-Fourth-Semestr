#include "mainwindow.h"

#include <QApplication>

//#include <QFile>
//#include <QSettings>

//#include <QString>

//void checkOrCreateSettings(const QString& fileName);

int main(int argc, char *argv[])
{
//  QSettings settings("settings.ini", QSettings::IniFormat);

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

//void checkOrCreateSettings(const QString &fileName){
//  if (QFile::exists(fileName)) return;


//}
