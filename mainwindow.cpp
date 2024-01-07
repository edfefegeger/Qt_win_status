#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>

// Подключаем заголовочные файлы и библиотеки для работы с Windows API
extern "C" {
#include <windows.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Устанавливаем соединение между событием нажатия кнопки и соответствующим методом
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обработчик события нажатия кнопки
void MainWindow::onPushButtonClicked()
{
    // Получаем текущий выбранный текст из выпадающего списка и текст из текстового поля
    QString currentText = ui->comboBox_2->currentText();
    QString user_text = ui->textEdit_2->toPlainText();

    // В зависимости от выбранного текста создаем событие с соответствующим типом
    if (currentText == "Warning")
    {
        reportEvent(user_text, user_text, EVENTLOG_WARNING_TYPE);
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Error")
    {
        reportEvent(user_text, user_text, EVENTLOG_ERROR_TYPE);
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Information")
    {
        reportEvent(user_text, user_text, EVENTLOG_INFORMATION_TYPE);
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Success Audit")
    {
        reportEvent(user_text, user_text, EVENTLOG_AUDIT_SUCCESS);
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
    else if (currentText == "Failure Audit")
    {
        reportEvent(user_text, user_text, EVENTLOG_AUDIT_FAILURE);
        ui->textEdit->append("Событие создано: " + currentText + "  " + user_text);
    }
}

// Создание и регистрация события в журнале событий Windows
void MainWindow::reportEvent(const QString& message, const QString& additionalText, WORD eventType)
{
    // Регистрируем источник события
    HANDLE hEventLog = RegisterEventSourceA(NULL, "MyEventSource");

    if (hEventLog != NULL)
    {
        // Подготавливаем сообщение для события
        const char* messages[] = { message.toStdString().c_str(), additionalText.toStdString().c_str() };

        // Регистрируем событие в журнале
        ReportEventA(hEventLog, eventType, 0, 0, NULL, 2, 0, reinterpret_cast<const CHAR**>(messages), NULL);

        // Закрываем источник события
        DeregisterEventSource(hEventLog);

        // Сохраняем информацию о событии в QStringList
        QString eventInfo = QString("Event Type: %1, Message: %2, Additional Text: %3")
                                .arg(eventType)
                                .arg(message)
                                .arg(additionalText);
        eventLog.append(eventInfo);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось зарегистрировать источник события.");
    }
}

// Метод для мониторинга событий в журнале событий Windows
void MainWindow::startEventLogMonitoring()
{
    // Открываем журнал событий
    HANDLE hEventLog = OpenEventLog(NULL, "Application");

    if (hEventLog != NULL)
    {
        EVENTLOGRECORD* pevlr;
        // Кастомное значение использованных байт, можете настроить под свою конфигурацию
        BYTE bBuffer[8192];
        DWORD dwRead, dwNeeded;

        pevlr = (EVENTLOGRECORD*)&bBuffer;

        // Очищаем список событий
        eventLog.clear();

        // Получаем все записи из журнала событий
        while (ReadEventLog(hEventLog,
                            EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
                            0,
                            pevlr,
                            sizeof(bBuffer),
                            &dwRead,
                            &dwNeeded))
        {
            // Получаем тип события
            WORD eventType = pevlr->EventType;

            // Получаем время события
            QDateTime eventTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(pevlr->TimeGenerated) * 1000);
            QString formattedTime = eventTime.toString("yyyy-MM-dd hh:mm:ss");

            // Получаем источник события
            QString source = QString(reinterpret_cast<const char*>((LPBYTE)pevlr + pevlr->StringOffset));

            // Добавляем событие в конец списка
            QString eventInfo = QString("Время события: %1, Тип события: %2, Источник: %3")
                                    .arg(formattedTime)
                                    .arg(eventType)
                                    .arg(source);
            eventLog.append(eventInfo);
        }

        // Обновляем интерфейс
        updateEventLogUI();

        // Закрываем журнал событий
        CloseEventLog(hEventLog);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть журнал событий.");
    }
}

// Метод для обновления интерфейса вывода событий
void MainWindow::updateEventLogUI()
{
    // Выводим все события в QTextEdit с пустой строкой между ними
    for (const QString &event : eventLog)
    {
        ui->textEdit->append(event);
        ui->textEdit->append("");
    }
}

// Обработчик события нажатия кнопки для мониторинга событий
void MainWindow::on_pushButton_2_clicked()
{
    startEventLogMonitoring();
    // Обновляем виджет QTextEdit новой информацией о событиях
}

// Обработчик события нажатия кнопки для очистки вывода
void MainWindow::on_pushButton_3_clicked()
{
    ui->textEdit->clear();
}
