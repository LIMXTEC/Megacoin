#ifndef SERVICEMESSAGEDIALOG_H
#define SERVICEMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
    class TransactionDescDialog;
}
QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

/** Dialog showing transaction details. */
class ServiceMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceMessageDialog(QWidget *parent = 0);
    ~ServiceMessageDialog();

    void setHtml(const QString& header, const QString& html);

private:
    Ui::TransactionDescDialog *ui;
};

#endif // SERVICEMESSAGEDIALOG_H
