#include "useritem.h"
#include "ui_useritem.h"
#include<QBitmap>
useritem::useritem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::useritem)
{
    ui->setupUi(this);
}

useritem::~useritem()
{
    delete ui;
}

void useritem::slot_setInfo(int id, QString name, int state, int icon, QString feeling)
{
    //1.保存用户信息
    m_id = id;
    m_name = name;
    m_state = state;
    m_icon = icon;
    m_feeling = feeling;
    //2.设置昵称和签名
    ui->lb_name->setText(m_name);
    ui->lb_feeling->setText(m_feeling);
    //3.判断用户是否在线
    //头像路径
    QString iconPath = QString(":/tx/%1.png").arg(icon);
    if(m_state)
    {
        //4.用户在线，显示彩色头像
        ui->pb_icon->setIcon(QIcon(iconPath));
    }
    else
    {
        //5.用户不在线，显示灰色头像
        QBitmap bmp; // 位图
        bmp.load(iconPath);
        ui->pb_icon->setIcon(bmp);//根据位图设置进去的头像是灰色的

    }
    //6.重绘
    this->repaint();
}



void useritem::on_pb_icon_clicked()
{
    Q_EMIT useritem::SIG_userItemClicker(m_id);
}
