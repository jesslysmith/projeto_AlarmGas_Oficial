#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSerialPort serial;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void dadosRecebidos();

    void on_btn_conectar_clicked();

    void on_btn_desconectar_clicked();

    void on_btn_desligasist_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
