#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>

extern "C" {
#include <windows.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPushButtonClicked()
{
    // Создаем и регистрируем событие в журнале событий
    reportEvent("Пример события", EVENTLOG_ERROR_TYPE);

    // Выводим событие в пользовательский интерфейс
    ui->textEdit->setPlainText("Событие создано: Пример события");  // Изменил на plainTextEdit
}

void MainWindow::reportEvent(const QString& message, WORD eventType)
{
    HANDLE hEventLog = RegisterEventSourceA(NULL, "MyEventSource");  // Изменил на RegisterEventSourceA

    if (hEventLog != NULL)
    {
        const char* messages[] = { message.toStdString().c_str() };  // Изменил на const char*

        ReportEventA(hEventLog, eventType, 0, 0, NULL, 1, 0, reinterpret_cast<const CHAR**>(messages), NULL);  // Изменил на ReportEventA

        DeregisterEventSource(hEventLog);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось зарегистрировать источник событий.");
    }
}

