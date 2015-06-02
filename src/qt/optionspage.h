#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QWidget>
#include <QVBoxLayout>

namespace Ui {
class OptionsPage;
}
class OptionsModel;
class MonitoredDataMapper;
class QValidatedLineEdit;

/** Preferences dialog. */
class OptionsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

    void showNotification(QVBoxLayout* layout, int position, const QString& text, bool canClose);

    void setModel(OptionsModel *model);
    void setMapper();

    void setCheckUpdatesAtStartup(bool bChecked);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    /* enable only apply button */
    void enableApplyButton();
    /* disable only apply button */
    void disableApplyButton();
    /* enable apply button and OK button */
    void enableSaveButtons();
    /* disable apply button and OK button */
    void disableSaveButtons();
    /* set apply button and OK button state (enabled / disabled) */
    void setSaveButtonState(bool fState);
    void on_resetButton_clicked();
//    void on_okButton_clicked();
//    void on_cancelButton_clicked();
    void on_applyButton_clicked();

    void showRestartWarning_Proxy();
    void showRestartWarning_Lang();
    void updateDisplayUnit();
    void handleProxyIpValid(QValidatedLineEdit *object, bool fState);

signals:
    void proxyIpValid(QValidatedLineEdit *object, bool fValid);

private:
    Ui::OptionsPage *ui;
    OptionsModel *model;
    MonitoredDataMapper *mapper;
    bool fRestartWarningDisplayed_Proxy;
    bool fRestartWarningDisplayed_Lang;
    bool fProxyIpValid;
};

#endif // OPTIONSDIALOG_H
