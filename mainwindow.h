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
    void reportEvent(const QString& message, const QString& additionalText, WORD eventType);
    void updateEventLogUI();
    void parseWindowsEventLog(QTextEdit* textEdit);
    void parseAndDisplayEvent(EVT_HANDLE hEvent, QTextEdit* textEdit);
    QString variantToString(const EVT_VARIANT& variant);
    void startEventLogMonitoring();
    void stopEventLogMonitoring();
    void onUpdateEventLog();

private:
    QStringList eventLog;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
