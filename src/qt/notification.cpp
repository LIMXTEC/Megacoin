#include "notification.h"
#include "ui_notification.h"

Notification::Notification(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Notification)
    , _isClosable(true)
{
    _ui->setupUi(this);
}

Notification::~Notification()
{
    delete _ui;
}

void Notification::setText(const QString &text)
{
    _ui->lbText->setText(text);
}

void Notification::setClosable(bool isClosable)
{
    _isClosable = isClosable;
    _ui->bClose->setVisible(_isClosable);
}

void Notification::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        _ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Notification::removeNotificationRequested()
{
    deleteLater();
    close();
}
