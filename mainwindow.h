#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Windows.h>

#include <QMainWindow>

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
    void reportEvent(const QString& message, WORD eventType);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
