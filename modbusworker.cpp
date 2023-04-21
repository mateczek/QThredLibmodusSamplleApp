#include "modbusworker.h"
modbusWorker::modbusWorker(modbusEngine *mb):mbe(mb)
{

}

modbusWorker::~modbusWorker()
{
     soc->disconnectFromHost();
     soc->deleteLater();
     if (modbusConnection!=nullptr) {
         modbus_free(modbusConnection);
         modbusConnection=nullptr;
     }

}

bool modbusWorker::start()
{
    bool retValue=false;
    modbusConnection=nullptr;
    soc = new QTcpSocket();
    soc->connectToHost(mbe->getip(),502);

    if(soc->waitForConnected(5000)){
        modbusConnection=modbus_new_tcp(soc->peerAddress().toString().toStdString().c_str(),soc->peerPort());
        modbus_set_socket(modbusConnection,soc->socketDescriptor());
        tim =new QTimer(this);
        tim->setInterval(1000);
        connect(tim,SIGNAL(timeout()),this,SLOT(cyclicRead()));
        tim->start();
        retValue=true;
    }
    return retValue;
}

void modbusWorker::writeWord(int Adres, int value)
{
    int stat = modbus_write_register(modbusConnection,Adres,value);
    if (stat==-1) emit modbusError();
}

void modbusWorker::writeBit(int Adres, int value)
{
    int stat = modbus_write_bit(modbusConnection,Adres,value);
    if (stat==-1) emit modbusError();
}

void modbusWorker::cyclicRead()
{
   int stat = modbus_read_registers(modbusConnection,mbe->getBaseReg(),mbe->getRegSize(),mbe->getRegpointer());
   if (stat==-1) {
       emit modbusError();
       return;
   }
   emit dataReaded();

}

