#ifndef MODBUSENGINE_H
#define MODBUSENGINE_H

#include <QThread>
#include <modbus/modbus-tcp.h>


class modbusEngine : public QThread
{
    Q_OBJECT
    uint16_t* regs;

    QString ip;
    int baseReg;
    int regSize;


public:
    explicit modbusEngine(uint16_t*_regs,QString _ip,int _baseReg,int _regSize, QObject *parent = nullptr);
    QString &getip(){return ip;}
    int getBaseReg(){return baseReg;}
    int getRegSize(){return regSize;}
    uint16_t* getRegpointer(){return regs;}

    void run();
signals:
    void dataReaded();
    void modbusOk(bool ok);
    void _writeBit(int, int);
    void _writeWord(int adress,int value);
public slots:
    void writeBit(int adr,int val){ emit _writeBit(adr,val);}
    void writeWord(int adress,int value){emit _writeWord(adress,value);}

};

#endif // MODBUSENGINE_H
