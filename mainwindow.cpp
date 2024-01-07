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

    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to register event source.");
    }
}
void MainWindow::startEventLogMonitoring()
{
    HANDLE hEventLog = OpenEventLog(NULL, "Application");

    if (hEventLog != NULL)
    {
        EVENTLOGRECORD* pevlr;
        BYTE bBuffer[16384];
        DWORD dwRead, dwNeeded;

        pevlr = (EVENTLOGRECORD*)&bBuffer;

        // Очищаем список событий
        eventLog.clear();

        // Сначала получаем общее количество записей в журнале
        DWORD dwRecordCount;
        if (GetNumberOfEventLogRecords(hEventLog, &dwRecordCount))
        {
            // Определите количество записей, которые вы хотите прочитать (например, 5)
            const int maxRecords = 5;
            int recordsRead = 0;

            // Затем получаем последние записи
            while (recordsRead < maxRecords &&
                   ReadEventLog(hEventLog,
                                 EVENTLOG_SEQUENTIAL_READ | EVENTLOG_BACKWARDS_READ,
                                 0,
                                 pevlr,
                                 sizeof(bBuffer),
                                 &dwRead,
                                 &dwNeeded))
            {
                // Получить тип события
                WORD eventType = pevlr->EventType;

                // Получить время события
                QDateTime eventTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(pevlr->TimeGenerated) * 1000);
                QString formattedTime = eventTime.toString("yyyy-MM-dd hh:mm:ss");

                // Получить источник события
                QString source = QString(reinterpret_cast<const char*>((LPBYTE)pevlr + pevlr->StringOffset));

                // Добавить последнее событие в список
                QString eventInfo = QString("Event Time: %1, Event Type: %2, Source: %3")
                                        .arg(formattedTime)
                                        .arg(eventType)
                                        .arg(source);
                eventLog.append(eventInfo);

                recordsRead++;
            }

            // Обновить интерфейс
            updateEventLogUI();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to get the number of event log records.");
        }

        CloseEventLog(hEventLog);
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to open EventLog.");
    }
}


void MainWindow::updateEventLogUI()
{
    // Display all events in the QTextEdit with spaces between them
    for (const QString &event : eventLog)
    {
        ui->textEdit->append(event);
        ui->textEdit->append("");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    startEventLogMonitoring();
    // Update the QTextEdit widget with the new event information

}


void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
}

