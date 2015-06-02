#ifndef SERVICEMESSAGESPAGE_H
#define SERVICEMESSAGESPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

namespace Ui {
    class ServiceMessagesPage;
}
class ClientModel;
class WalletModel;

class ServiceMessagesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceMessagesPage(QWidget *parent = 0);
    ~ServiceMessagesPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

private slots:
    void handleClicked(const QModelIndex &index);

private:
    Ui::ServiceMessagesPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;

};

#endif // SERVICEMESSAGESPAGE_H
