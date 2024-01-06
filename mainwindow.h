#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Windows.h>
#include <QMainWindow>
#include <QTextEdit>
#include <QStringList>
extern "C" {
#include <windows.h>
#include <winevt.h>
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPushButtonClicked();
    void onUpdateEventLog();

private:
    Ui::MainWindow *ui;
    QStringList eventLog;

    void reportEvent(const QString &message, const QString &additionalText, const QString &eventType);
    void updateEventLogUI();
    void startEventLogMonitoring();
    void stopEventLogMonitoring();
    void parseAndDisplayEvent(void *hEvent, QTextEdit *textEdit);
};

#endif // MAINWINDOW_H
