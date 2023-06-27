#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
signals:
    //发送注册信号
    void SIG_registerCommit(QString name,QString tel,QString password);
    //发送登录信号
    void SIG_loginCommit(QString tel,QString password);
private slots:
    void on_pb_clear_register_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_clicked();

    void on_pb_commit_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
