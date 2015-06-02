#include "servicemessagespage.h"
#include "ui_servicemessagespage.h"

#include "servicemessagedialog.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "megacoinunits.h"
#include "optionsmodel.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QStandardItemModel>
#include <QAbstractItemDelegate>
#include <QPainter>

extern QFont g_fontHeader;

ServiceMessagesPage::ServiceMessagesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceMessagesPage),
    clientModel(0),
    walletModel(0)
{
    ui->setupUi(this);

    connect(ui->tableServiceMessagesView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(handleClicked(QModelIndex)));

    QColor foreground(qRgb(171, 127, 130));
    QStandardItemModel* model = new QStandardItemModel(2, 2, this);

    for (int i = 0; i < 0; ++i)
    {
        QStandardItem* item = new QStandardItem(trUtf8("17:56, 01.03.2012"));
        item->setForeground(foreground);
        model->setItem(i, 0, item);

        item = new QStandardItem(QString::fromUtf8(
                                     i == 0 ?
                                         "":
                                         ""));
        item->setForeground(foreground);
        model->setItem(i, 1, item);
    }

    ui->tableServiceMessagesView->setModel(model);
    ui->tableServiceMessagesView->setColumnWidth(0, 120);
    ui->tableServiceMessagesView->horizontalHeader()->setStretchLastSection(true);
    ui->tableServiceMessagesView->resizeRowsToContents();
}

void ServiceMessagesPage::handleClicked(const QModelIndex &index)
{
    int row = index.row();
    ServiceMessageDialog dlg;
    if (row == 0)
        dlg.setHtml("Header", "");
    else
        dlg.setHtml("Header 2", "");
    dlg.exec();
}

ServiceMessagesPage::~ServiceMessagesPage()
{
    delete ui;
}


void ServiceMessagesPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void ServiceMessagesPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
}
