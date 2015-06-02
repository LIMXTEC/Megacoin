#include "transactiondescdialog.h"
#include "ui_transactiondescdialog.h"

#include "transactiontablemodel.h"

#include <QDesktopWidget>
#include <QModelIndex>

TransactionDescDialog::TransactionDescDialog(const QModelIndex &idx, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDescDialog)
{
    ui->setupUi(this);
    QString desc = idx.data(TransactionTableModel::LongDescriptionRole).toString();
    ui->detailText->setHtml(desc);

    // Center window (deleted)
//    QRect scr = QApplication::desktop()->screenGeometry();
//    move(scr.center() - rect().center());
}

TransactionDescDialog::~TransactionDescDialog()
{
    delete ui;
}
