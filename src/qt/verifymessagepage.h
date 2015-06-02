#ifndef VERIFYMESSAGEPAGE_H
#define VERIFYMESSAGEPAGE_H

#include <QWidget>

namespace Ui {
    class VerifyMessagePage;
}
class WalletModel;

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class VerifyMessagePage : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyMessagePage(QWidget *parent = 0);
    ~VerifyMessagePage();

    void setModel(WalletModel *model);
    void setAddress_VM(const QString &address);

    void showTab_VM(bool fShow);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::VerifyMessagePage *ui;
    WalletModel *model;

private slots:
    /* verify message */
    void on_addressBookButton_VM_clicked();
    void on_verifyMessageButton_VM_clicked();
    void on_clearButton_VM_clicked();
};

#endif // VERIFYMESSAGEPAGE_H
