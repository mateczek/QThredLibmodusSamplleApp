#include "modbusengine.h"
#include <modbusworker.h>

modbusEngine::modbusEngine(uint16_t*_regs,QString _ip,int _baseReg,int _regSize,QObject *parent)
    : QThread{parent}
{
        regs=_regs;
        ip=_ip;
        baseReg=_baseReg;
        regSize=_regSize;
}

void modbusEngine::run() //oddzielny wątek dla komunikacji modbus
{
    modbusWorker worker(this); //przekazanie wskaźnika na klase modbusEngine do obiektu worker by mógł korzystać z funkcji
    if (worker.start()){
        emit modbusOk(true);
        connect(&worker,SIGNAL(dataReaded()),this,SIGNAL(dataReaded())); //przekazanie sygnału odczytane dane z obiektu work dalej
        connect(this,SIGNAL(_writeBit(int,int)),&worker,SLOT(writeBit(int,int)));
        connect(this,SIGNAL(_writeWord(int,int)),&worker,SLOT(writeWord(int,int)));
        connect(&worker,SIGNAL(modbusError()),this,SLOT(quit())); //błąd równa koniec wątku
        exec();
    }
    emit modbusOk(false);
}
