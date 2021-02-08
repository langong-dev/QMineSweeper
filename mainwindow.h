#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include "block.h"
#include <QTimer>
#include <QMessageBox>
#include <qmessagebox.h>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QPushButton>
#include <QDesktopServices>
#include "inputdialog.h"

#define BLOCK_LENGTH 21
#define BLOCK_HEIGH 21
#define ELENUM_LENGHT 20
#define EXPRESSION_LENGHT 24
#define EXPRESSION_HEIGH 24
#define TTILE_BEGIN 23
#define TITILE_HEIGH  (TTILE_BEGIN+40)
#define PRIMARY_LENGTH  189
#define MIDDLE_LENGTH   336
#define TOP_LENGTH      630

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTimer * runtime;
//    Dialog * useDefineWindows;
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private slots:
    void on_secondadd();

    void Easy_triggered();
    void Middle_triggered();
    void Hard_triggered();
    void Exit_triggered();
    void Customize_triggered();
    void AboutQT_triggered();
    void About_triggered();

private:
    Ui::MainWindow *ui;
};

void showblock(int x,int y);
int iswin();
void restart(int row,int col,int boolnum);
void painttitle(QPainter * painter);
void paintboom(QPainter * painter);
void createinterface();

#endif // MAINWINDOW_H
