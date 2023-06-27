#include "logindialog.h"
#include "ui_logindialog.h"
#include<qmessagebox.h>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_tel_register->setText("");;
    ui->le_name_register->setText("");
    ui->le_password_register->setText("");
}

void LoginDialog::on_pb_commit_register_clicked()
{
    //1.获取到用户输入的数据
    QString name = ui->le_name_register->text();
    QString tel = ui->le_tel_register->text();
    QString password = ui->le_password_register->text();
    //2.校验用户数据的有效性
    //2.1校验字符串不能位空，不能为空格
    QString nameTemp = name;
    QString telTemp = tel;
    QString passwordTemp = password;

    if(name.isEmpty() || tel.isEmpty() || password.isEmpty()
            || nameTemp.remove(" ").isEmpty() || telTemp.remove(" ").isEmpty()
            || passwordTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","昵称，电话号码，密码不能为空或者空格");
        return ;
    }
    //2.2校验长度，昵称不能超过10位，密码不能超过20，电话号码是11
    if(name.length() > 10 || tel.length() != 11 || password.length() > 20)
    {
        QMessageBox::about(this,"提示","昵称长度不能超过10，电话号码必须为11位，密码长度不能超过20");
        return ;
    }

    //2.3校验内容，电话号只能有数字，-->需要使用正则表达式，暂时不做
    //3.把数据发给kernel类
    Q_EMIT SIG_registerCommit(name,tel,password);
    //
}

void LoginDialog::on_pb_clear_clicked()
{
    ui->le_tel->setText("");
    ui->le_password->setText("");

}

void LoginDialog::on_pb_commit_clicked()
{
    //1.获取到用户输入的数据
    QString tel = ui->le_tel->text();
    QString password = ui->le_password->text();
    //2.校验用户数据的有效性
    //2.1校验字符串不能位空，不能为空格
    QString telTemp = tel;
    QString passwordTemp = password;

    if(tel.isEmpty() || password.isEmpty()|| telTemp.remove(" ").isEmpty()
            || passwordTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","电话号码，密码不能为空或者空格");
        return ;
    }
    //2.2校验长度，密码不能超过20，电话号码是11
    if(tel.length() != 11 || password.length() > 20)
    {
        QMessageBox::about(this,"提示","电话号码必须为11位，密码长度不能超过20");
        return ;
    }

    //2.3校验内容，电话号只能有数字，-->需要使用正则表达式，暂时不做
    //3.把数据发给kernel类
    Q_EMIT SIG_loginCommit(tel,password);
    //
}
