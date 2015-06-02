#include "servicemessagedialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"

#include <QDesktopWidget>
#include <QModelIndex>

ServiceMessageDialog::ServiceMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDescDialog)
{
    ui->setupUi(this);

    // Center window (deleted)
//    QRect scr = QApplication::desktop()->screenGeometry();
//    move(scr.center() - rect().center());
}

ServiceMessageDialog::~ServiceMessageDialog()
{
    delete ui;
}

void ServiceMessageDialog::setHtml(const QString& header, const QString &html)
{
    setWindowTitle(header);
    ui->detailText->setHtml(html);
}
