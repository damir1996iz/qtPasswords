#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QListWidget>
#include <QtGui>

#include <QList>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

#include "passwordrecord.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readdb();
private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    QList <PasswordRecord>passwords;

public slots:
    void onListItemClicked(QListWidgetItem *itm);
    void onAddBtnClicked();
    void onDeleteBtnClicked();
};
#endif // MAINWINDOW_H
