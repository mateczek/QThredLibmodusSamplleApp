#ifndef MODBUSWORKER_H
#define MODBUSWORKER_H

#include <QObject>
#include<QTimer>
#include<modbusengine.h>
#include<modbus/modbus-tcp.h>
#include<QTcpSocket>
class modbusWorker : public QObject
{
    Q_OBJECT
     modbusEngine *mbe;
     QTimer* tim;
     modbus_t*modbusConnection;
     QTcpSocket* soc;

public:
    explicit modbusWorker(modbusEngine *mb);
    ~modbusWorker();
     bool start();

signals:
     void dataReaded();
     void modbusError();

public slots:
      void writeBit(int Adres,int value);
      void writeWord(int Adres,int value);
private slots:
      void cyclicRead();
};

#endif // MODBUSWORKER_H
