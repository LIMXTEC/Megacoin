#ifndef SIGNMESSAGEPAGE_H
#define SIGNMESSAGEPAGE_H

#include <QWidget>

namespace Ui {
    class SignMessagePage;
}
class WalletModel;

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class SignMessagePage : public QWidget
{
    Q_OBJECT

public:
    explicit SignMessagePage(QWidget *parent = 0);
    ~SignMessagePage();

    void setModel(WalletModel *model);
    void setAddress_SM(const QString &address);

    void showTab_SM(bool fShow);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::SignMessagePage *ui;
    WalletModel *model;

private slots:
    /* sign message */
    void on_addressBookButton_SM_clicked();
    void on_pasteButton_SM_clicked();
    void on_signMessageButton_SM_clicked();
    void on_copySignatureButton_SM_clicked();
    void on_clearButton_SM_clicked();
};

#endif // SIGNMESSAGEPAGE_H
