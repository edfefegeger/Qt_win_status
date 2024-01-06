// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onPushButtonClicked);

    // Инициализация таймера для периодического обновления журнала
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onUpdateEventLog);
    timer->start(5000); // Обновление каждые 5 секунд (можно изменить интервал по вашему усмотрению)

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
    QString userText = ui->textEdit_2->toPlainText();

    // Создаем и регистрируем событие в журнале
    reportEvent("Пример события", userText, currentText);

    // Выводим событие в пользовательский интерфейс
    ui->textEdit->append("Событие создано: " + currentText + "  " + userText);
}

void MainWindow::reportEvent(const QString &message, const QString &additionalText, const QString &eventType)
{
    // Получаем текущую дату и время
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Создаем строку для записи
    QString logEntry = QString("[%1] %2: %3").arg(timestamp, eventType, additionalText);

    // Добавляем событие в список
    eventLog.append(logEntry);

    // В данном примере мы не чистим список, но в реальном приложении вам, возможно, придется ограничивать его размер
}

void MainWindow::updateEventLogUI()
{
    // Выводим все события в QTextEdit
    for (const QString &event : eventLog)
    {
        ui->textEdit->append(event);
    }
}

void MainWindow::startEventLogMonitoring()
{
    // Запуск мониторинга событий (если необходимо)
    // В данном примере этот метод может быть пустым, поскольку мониторинг реализован через таймер
}

void MainWindow::stopEventLogMonitoring()
{
    // Остановка мониторинга событий (если необходимо)
    // В данном примере этот метод может быть пустым, поскольку мониторинг реализован через таймер
}

void MainWindow::onUpdateEventLog()
{
    // Обновление журнала событий
    updateEventLogUI();
}
