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
    QString currentText = ui->comboBox_2->currentText();
    QString user_text = ui->textEdit_2->toPlainText();

    if (currentText == "Warning")
    {
        // Создаем и регистрируем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_WARNING_TYPE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Error")
    {
        // Создаем и регистрируем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_ERROR_TYPE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Information")
    {
        // Создаем и регистрируем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_INFORMATION_TYPE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Success Audit")
    {
        // Создаем и регистрируем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_AUDIT_SUCCESS);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Failure Audit")
    {
        // Создаем и регистрируем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_AUDIT_FAILURE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
}



void MainWindow::reportEvent(const QString& message, const QString& additionalText, WORD eventType)
{
    HANDLE hEventLog = RegisterEventSourceA(NULL, "MyEventSource");

    if (hEventLog != NULL)
    {
        const char* messages[] = { message.toStdString().c_str(), additionalText.toStdString().c_str() };

        ReportEventA(hEventLog, eventType, 0, 0, NULL, 2, 0, reinterpret_cast<const CHAR**>(messages), NULL);

        DeregisterEventSource(hEventLog);

        // Store the event information in the eventLog QStringList
        QString eventInfo = QString("Event Type: %1, Message: %2, Additional Text: %3")
                                .arg(eventType)
                                .arg(message)
                                .arg(additionalText);
        eventLog.append(eventInfo);

        // Update the QTextEdit widget with the new event information
        updateEventLogUI();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to register event source.");
    }
}

void MainWindow::updateEventLogUI()
{
    // Clear existing content in the QTextEdit
    ui->textEdit->clear();

    // Display all events in the QTextEdit
    for (const QString &event : eventLog)
    {
        ui->textEdit->append(event);
    }
}
