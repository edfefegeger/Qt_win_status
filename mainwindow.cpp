#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QTimer>

extern "C" {
#include <windows.h>
#include <winevt.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);

    // Инициализация таймера для периодического обновления журнала
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdateEventLog);
    timer->start(5000);  // Обновление каждые 5 секунд (можно изменить интервал по вашему усмотрению)

    // Запуск мониторинга событий из журнала
    startEventLogMonitoring();
}

MainWindow::~MainWindow()
{
    // Остановка мониторинга событий при закрытии приложения
    stopEventLogMonitoring();

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
        // Создаем и регистриуем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_ERROR_TYPE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Information")
    {
        // Создаем и регистриуем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_INFORMATION_TYPE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Success Audit")
    {
        // Создаем и регистриуем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_AUDIT_SUCCESS);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Failure Audit")
    {
        // Создаем и регистриуем событие в журнале событий
        reportEvent("Пример события", user_text, EVENTLOG_AUDIT_FAILURE);
        // Выводим событие в пользовательский интерфейс
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
}

void MainWindow::reportEvent(const QString& message, const QString& additionalText, WORD eventType)
{
    HANDLE hEventLog = RegisterEventSourceA(NULL, "MyEventSource");  // Изменил на RegisterEventSourceA

    if (hEventLog != NULL)
    {
        const char* messages[] = { message.toStdString().c_str(), additionalText.toStdString().c_str() };

        ReportEventA(hEventLog, eventType, 0, 0, NULL, 2, 0, reinterpret_cast<const CHAR**>(messages), NULL);

        DeregisterEventSource(hEventLog);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось зарегистрировать источник событий.");
    }
}

void MainWindow::updateEventLogUI()
{
    // Выводим все пользовательские события в QTextEdit
    for (const QString& event : eventLog)
    {
        ui->textEdit->append(event);
    }

    // Выводим все события Windows Event Log
    parseWindowsEventLog(ui->textEdit);
}

void MainWindow::parseWindowsEventLog(QTextEdit* textEdit)
{
    EVT_HANDLE hEventLog = EvtOpenLog(NULL, L"System", EvtOpenChannelPath | EvtOpenBatchRead);

    if (hEventLog != NULL)
    {
        // Определение фильтра для событий
        EVT_HANDLE hEventBookmark = EvtCreateBookmark(NULL);
        DWORD dwBufferSize = 0;
        BYTE* pBuffer = NULL;

        if (!EvtRender(hEventLog, hEventBookmark, EvtRenderBookmark, dwBufferSize, pBuffer, &dwBufferSize, NULL))
        {
            if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
            {
                pBuffer = (BYTE*)malloc(dwBufferSize);
                if (pBuffer)
                {
                    if (!EvtRender(hEventLog, hEventBookmark, EvtRenderBookmark, dwBufferSize, pBuffer, &dwBufferSize, NULL))
                    {
                        free(pBuffer);
                        pBuffer = NULL;
                    }
                }
            }
        }

        // Задаем фильтр
        if (EvtSubscribe(hEventLog, NULL, NULL, NULL, hEventBookmark, NULL, EvtSubscribeToFutureEvents, NULL, NULL))

        {
            // Чтение и вывод событий
            EVT_HANDLE hEvents[10];
            DWORD dwReturned = 0;

            while (EvtNext(hEventLog, 10, hEvents, INFINITE, 0, &dwReturned))
            {
                for (DWORD i = 0; i < dwReturned; i++)
                {
                    // Обработка и вывод информации о событии
                    parseAndDisplayEvent(hEvents[i], textEdit);

                    // Закрытие дескриптора события
                    EvtClose(hEvents[i]);
                }
            }

            // Отмена подписки на события
            EvtClose(hEventLog);
            EvtClose(hEventBookmark);
            free(pBuffer);
        }
        else
        {
            QMessageBox::critical(this, "Ошибка", "Не удалось подписаться на события Windows Event Log.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть журнал событий Windows.");
    }
}

void MainWindow::parseAndDisplayEvent(EVT_HANDLE hEvent, QTextEdit* textEdit)
{
    // Добавьте код обработки и вывода информации о событии здесь
    // Заглушка, чтобы избежать ошибок компиляции
    textEdit->append("Обработка и вывод информации о событии");
}

QString MainWindow::variantToString(const EVT_VARIANT& variant)
{
    // Добавьте код преобразования в строку для различных типов данных
    // Заглушка, чтобы избежать ошибок компиляции
    return "Значение варианта";
}

void MainWindow::startEventLogMonitoring()
{
    // Запуск мониторинга событий Windows Event Log
    // Вы можете добавить здесь код для создания потока или другого механизма мониторинга
    // На каждом шаге мониторинга вызывайте parseWindowsEventLog для обновления данных
    // В этом примере просто вызываем parseWindowsEventLog в таймере

    //parseWindowsEventLog(ui->textEdit); // Можно закомментировать, если будет использоваться в потоке
}

void MainWindow::stopEventLogMonitoring()
{
    // Остановка мониторинга событий Windows Event Log
    // Вы можете добавить здесь код для остановки потока или другого механизма мониторинга
}

void MainWindow::onUpdateEventLog()
{
    // Обновление журнала событий
    parseWindowsEventLog(ui->textEdit);
}
