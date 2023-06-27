#include "imserverdialog.h"
#include "ui_imserverdialog.h"

IMServerDialog::IMServerDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IMServerDialog)
{
    ui->setupUi(this);
    setWindowTitle("通讯软件1.0");
    //初始化垂直布局的层
    m_layout = new QVBoxLayout;
    //距离外面控件左，上，右，下的边距
    m_layout->setContentsMargins(0,0,0,0);
    //设置每个控件的 间距
    m_layout->setSpacing(3);
    //设置控件的层
    ui->wdg_list->setLayout(m_layout);
    //创建菜单对象，指定父控件,父控件负责子控件回收
    m_menu = new QMenu(this);
    //添加菜单项
    m_menu->addAction("添加好友");
    m_menu->addAction("系统设置");
    //绑定点击菜单的信号和槽函数
    //triggered 是QMenu 类里的信号，我们不需要负责定义发送，点击菜单的时候就会发送
    connect(m_menu,SIGNAL(triggered(QAction*)),
            this,SLOT(slot_dealMenu(QAction*)));
}

IMServerDialog::~IMServerDialog()
{
    delete ui;
}
//添加好友
void IMServerDialog::slot_addFriend(useritem *item)
{
    //往层（layout）上添加好友控件
    m_layout->addWidget(item);
}

void IMServerDialog::slot_setInfo(QString name, QString feeling, int icon)
{
    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);

    QString iconPath = QString(":/tx/%1.png").arg(icon);
    ui->pb_icon->setIcon(QIcon(iconPath));
////    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(icon)));


}
//弹出菜单栏
void IMServerDialog::on_pb_menu_clicked()
{
    //显示菜单
    //先获取鼠标点击的位置，在鼠标点击的位置上方显示菜单
    QPoint p = QCursor::pos();
    //求出菜单的绝对高----所有像素点完成后的高度
    QSize size = m_menu->sizeHint();
    m_menu->exec(QPoint(p.x(),p.y()-size.height()));
}
//处理菜单点击事件的槽函数
void IMServerDialog::slot_dealMenu(QAction* action)
{
    if("添加好友" == action->text())
    {
        //给kernel发送添加好友信号
        Q_EMIT SIG_AddFriend();
    }
    else if("系统设置" == action->text())
    {

    }
}
