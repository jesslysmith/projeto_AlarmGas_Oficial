#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&serial,
            SIGNAL(readyRead()),
            this,
            SLOT(dadosRecebidos()));

    for(auto& item : QSerialPortInfo::availablePorts())
        ui->box_serial->addItem(item.portName());

    for(auto& item : QSerialPortInfo::standardBaudRates())
        ui->box_velocidade->addItem(QString::number(item) );
}

MainWindow::~MainWindow()
{

    delete ui;
}



void MainWindow::on_btn_conectar_clicked()
{
    serial.setPortName(ui->box_serial->currentText());
    serial.setBaudRate(ui->box_velocidade->currentText().toInt());

    if(serial.open(QIODevice::ReadWrite)){
        ui->lbl_status->setText("Status: Conectado");
    }
}

void MainWindow::on_btn_desconectar_clicked()
{
    serial.close();
    ui->lbl_status->setText("Status: Desconectado");
}

void MainWindow::dadosRecebidos()
{
    auto data = serial.readAll();
    auto dados = QJsonDocument::fromJson(data).object().toVariantMap();

    //qDebug() << data;
    if(dados.contains("LED")){
        //qDebug() << "Entrei";
        if(dados["LED"] == 1){
            ui->lbl_lcd->setText("PERIGO! NÍVEL ALARMANTE!");
            ui->label->setEnabled(1);
        }
        else{
            ui->lbl_lcd->setText("Em funcionamento...");
            ui->label->setEnabled(0);
        }
    }

    if(dados.contains("SENSOR")){
        ui->lbl_concentracao->setText(dados["SENSOR"].toString());
    }
}

void MainWindow::on_btn_desligasist_clicked()
{
    serial.write("{\"T_OFF\": 0}\n");
    ui->lbl_lcd->setText("Em funcionamento...");
    ui->label->setEnabled(0);
}
