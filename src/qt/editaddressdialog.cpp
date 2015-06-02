#include "editaddressdialog.h"
#include "ui_editaddressdialog.h"

#include "addresstablemodel.h"
#include "guiutil.h"

#include <QClipboard>
#include <QDesktopWidget>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include "dialog_move_handler.h"

EditAddressDialog::EditAddressDialog(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAddressDialog), mapper(0), mode(mode), model(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Window);
    ui->wCaption->installEventFilter(new DialogMoveHandler(this));

    GUIUtil::setupAddressWidget(ui->addressEdit, this);

    switch(mode)
    {
    case NewReceivingAddress:
        ui->lbTitle->setText(tr("New receiving address"));
        setWindowTitle(tr("New receiving address"));
        ui->addressEdit->setEnabled(false);
        ui->pasteButton->setEnabled(false);
        ui->picEdit->setVisible(false);
        ui->picAdd->setVisible(true);
        break;
    case NewSendingAddress:
        ui->lbTitle->setText(tr("New sending address"));
        setWindowTitle(tr("New sending address"));
        ui->picEdit->setVisible(false);
        ui->picAdd->setVisible(true);
        break;
    case EditReceivingAddress:
        ui->lbTitle->setText(tr("Edit receiving address"));
        setWindowTitle(tr("Edit receiving address"));
        ui->addressEdit->setEnabled(false);
        ui->pasteButton->setEnabled(false);
        ui->picEdit->setVisible(true);
        ui->picAdd->setVisible(false);
        break;
    case EditSendingAddress:
        ui->lbTitle->setText(tr("Edit sending address"));
        setWindowTitle(tr("Edit sending address"));
        ui->picEdit->setVisible(true);
        ui->picAdd->setVisible(false);
        break;
    }

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    QRect scr = QApplication::desktop()->screenGeometry();
    move(scr.center() - rect().center());
}

EditAddressDialog::~EditAddressDialog()
{
    delete ui;
}

void EditAddressDialog::setModel(AddressTableModel *model)
{
    this->model = model;
    if(!model)
        return;

    mapper->setModel(model);
    mapper->addMapping(ui->labelEdit, AddressTableModel::Label);
    mapper->addMapping(ui->addressEdit, AddressTableModel::Address);
}

void EditAddressDialog::loadRow(int row)
{
    mapper->setCurrentIndex(row);
}

bool EditAddressDialog::saveCurrentRow()
{
    if(!model)
        return false;

    switch(mode)
    {
    case NewReceivingAddress:
    case NewSendingAddress:
        address = model->addRow(
                mode == NewSendingAddress ? AddressTableModel::Send : AddressTableModel::Receive,
                ui->labelEdit->text(),
                ui->addressEdit->text());
        break;
    case EditReceivingAddress:
    case EditSendingAddress:
        if(mapper->submit())
        {
            address = ui->addressEdit->text();
        }
        break;
    }
    return !address.isEmpty();
}

void EditAddressDialog::accept()
{
    if(!model)
        return;

    if(!saveCurrentRow())
    {
        switch(model->getEditStatus())
        {
        case AddressTableModel::OK:
            // Failed with unknown reason. Just reject.
            break;
        case AddressTableModel::NO_CHANGES:
            // No changes were made during edit operation. Just reject.
            break;
        case AddressTableModel::INVALID_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
                tr("The entered address \"%1\" is not a valid Megacoin address.").arg(ui->addressEdit->text()),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::DUPLICATE_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
                tr("The entered address \"%1\" is already in the address book.").arg(ui->addressEdit->text()),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::WALLET_UNLOCK_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("Could not unlock wallet."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::KEY_GENERATION_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("New key generation failed."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;

        }
        return;
    }
    QDialog::accept();
}

void EditAddressDialog::on_pasteButton_clicked()
{
    // Paste text from clipboard into recipient field
    ui->addressEdit->setText(QApplication::clipboard()->text());
}


QString EditAddressDialog::getAddress() const
{
    return address;
}

void EditAddressDialog::setAddress(const QString &address)
{
    this->address = address;
    ui->addressEdit->setText(address);
}
