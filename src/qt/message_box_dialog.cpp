#include "message_box_dialog.h"
#include "ui_message_box_dialog.h"
#include <QDebug>
#include "dialog_move_handler.h"

MessageBoxDialog::MessageBoxDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageBoxDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Window);

    ui->wCaption->installEventFilter(new DialogMoveHandler(this));
}

MessageBoxDialog::~MessageBoxDialog()
{
    delete ui;
}

int MessageBoxDialog::showMessage(const QString &title, const QString &text, MessageBoxDialog::ETYPE type)
{
    QPixmap pixmap;
    switch (type)
    {
    case E_INFORMATION:
        pixmap.load(":/res/information_icon.png");
        ui->wYesNoHolder->setVisible(false);
        ui->wCheckboxHolder->setVisible(false);
        break;
    case E_RED_ALERT:
        pixmap.load(":/res/warning_icon.png");
        ui->wYesNoHolder->setVisible(false);
        ui->wCheckboxHolder->setVisible(false);
        break;
    case E_YES_NO:
        pixmap.load(":/res/information_icon.png");
        ui->wAcceptHolder->setVisible(false);
        ui->wCheckboxHolder->setVisible(false);
        break;
    case E_YES_NO_CHECKBOX:
        pixmap.load(":/res/information_icon.png");
        ui->wAcceptHolder->setVisible(false);
        break;
    }

    ui->lbTitle->setText(title);
    ui->lbTitleIcon->setPixmap(pixmap);
    ui->lbMainText->setText(text);

    adjustSize();

    return exec();
}

bool MessageBoxDialog::getCheckboxStatusChecked()
{
    return ui->cbDontShow->isChecked();
}

void MessageBoxDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
