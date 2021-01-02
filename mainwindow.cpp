#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(800,600);

    connect(ui->listWidget,&QListWidget::itemClicked,this,&MainWindow::onListItemClicked);
    connect(ui->pushButton,&QPushButton::pressed,this,&MainWindow::onAddBtnClicked);
    connect(ui->pushButton_2,&QPushButton::pressed,this,&MainWindow::onDeleteBtnClicked);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("pwd");
    db.open();

    readdb();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readdb()
{
    passwords.clear();

    QSqlQuery q;
    q.exec("SELECT * FROM 'passwords'");

    while(q.next())
    {
        PasswordRecord pwd;
        pwd.id = q.value(0).toInt();
        pwd.site = q.value(1).toString();
        pwd.user = q.value(2).toString();
        pwd.password = q.value(3).toString();
        pwd.additional = q.value(4).toString();
        pwd.decrypt();

        passwords.append(pwd);
    }

    ui->listWidget->clear();

    foreach(PasswordRecord rec,passwords)
    {
        ui->listWidget->addItem(rec.site);
    }
}


void MainWindow::onListItemClicked(QListWidgetItem* itm)
{
    ui->lineEdit->setText(passwords.at(ui->listWidget->currentRow()).site);
    ui->lineEdit_2->setText(passwords.at(ui->listWidget->currentRow()).user);
    ui->lineEdit_3->setText(passwords.at(ui->listWidget->currentRow()).password);
    ui->plainTextEdit->setPlainText(passwords.at(ui->listWidget->currentRow()).additional);
    ui->label_5->setText(QString::number(passwords.at(ui->listWidget->currentRow()).id));
}

void MainWindow::onAddBtnClicked()
{
    QSqlQuery q;
    QString values = "";
    PasswordRecord pwd;
    pwd.site = ui->lineEdit->text();
    pwd.user = ui->lineEdit_2->text();
    pwd.password = ui->lineEdit_3->text();
    pwd.additional = ui->plainTextEdit->toPlainText();

    pwd.crypt();

    values += "(NULL,'" + pwd.site +"','"+pwd.user+"','"+pwd.password+"','"+pwd.additional+"')";

    QMessageBox msg;
    if(q.exec("INSERT INTO 'passwords' VALUES "+values))
    {
        msg.setText("Successful!");
    }
    else
    {
        msg.setText("ERROR:INSERT INTO 'passwords' VALUES "+values);
    }
    msg.exec();
    readdb();
}

void MainWindow::onDeleteBtnClicked()
{
    QSqlQuery q;
    QMessageBox msg;

    if(q.exec("DELETE FROM 'passwords' WHERE id='"+ui->label_5->text()+"'"))
    {
        msg.setText("Successful!");
    }
    else
    {
        msg.setText("ERROR: DELETE FROM 'passwords' WHERE id='"+ui->label_5->text()+"'");
    }
    msg.exec();
    readdb();
}
