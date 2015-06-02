/*
 * Qt4 megacoin GUI.
 *
 * W.J. van der Laan 2011-2012
 * The Megacoin Developers 2011-2012
 */

#include "megacoingui.h"
#include "ui_mainwindow.h"
#include "transactiontablemodel.h"
#include "addressbookpage.h"
#include "sendcoinsdialog.h"
#include "miningpage.h"
#include "dialog_move_handler.h"
#include "optionspage.h"
#include "aboutdialog.h"
#include "servicemessagespage.h"
#include "signmessagepage.h"
#include "verifymessagepage.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "editaddressdialog.h"

#include "optionsmodel.h"
#include "transactiondescdialog.h"
#include "addresstablemodel.h"
#include "transactionview.h"
#include "overviewpage.h"
#include "megacoinunits.h"
#include "guiconstants.h"
#include "askpassphrasedialog.h"
#include "notificator.h"
#include "guiutil.h"
#include "rpcconsole.h"
#include "ui_interface.h"
#include "main.h"
#include "init.h"
#include "util.h"
#include "message_box_dialog.h"

#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLocale>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>
#include <QDateTime>
#include <QMovie>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QDragEnterEvent>
#if QT_VERSION < 0x050000
#include <QUrl>
#endif
#include <QMimeData>
#include <QStyle>
#include <QSettings>
#include <QDesktopWidget>
#include <QListWidget>
#include <QPainter>
#include <QSound>
#include <QSizeGrip>

#include <iostream>

const QString MegacoinGUI::DEFAULT_WALLET = "~Default";

MegacoinGUI::MegacoinGUI(bool fIsTestnet, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    clientModel(0),
    walletModel(0),
    encryptWalletAction(0),
    changePassphraseAction(0),
    aboutQtAction(0),
    trayIcon(0),
    notificator(0),
    rpcConsole(0),
    prevBlocks(0)
{
    //restoreWindowGeometry();

    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Window);

    ui->wCaption->installEventFilter(new DialogMoveHandler(this));

#ifndef Q_OS_MAC
    if (!fIsTestnet)
    {
        setWindowTitle(tr("Megacoin") + " - " + tr("Wallet"));
        QApplication::setWindowIcon(QIcon(":icons/megacoin"));
        setWindowIcon(QIcon(":icons/megacoin"));
    }
    else
    {
        setWindowTitle(tr("Megacoin") + " - " + tr("Wallet") + " " + tr("[testnet]"));
        QApplication::setWindowIcon(QIcon(":icons/megacoin_testnet"));
        setWindowIcon(QIcon(":icons/megacoin_testnet"));
    }
#else
    setUnifiedTitleAndToolBarOnMac(true);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);

    if (!fIsTestnet)
        MacDockIconHandler::instance()->setIcon(QIcon(":icons/megacoin"));
    else
        MacDockIconHandler::instance()->setIcon(QIcon(":icons/megacoin_testnet"));
#endif
    // Accept D&D of URIs
    setAcceptDrops(true);

    // Create actions for the toolbar, menu bar and tray/dock icon
    // Needs walletFrame to be initialized
    createActions(fIsTestnet);

    // Create application menu bar
    //$
    // createMenuBar();

    // Create the toolbars
    //$
    // createToolBars();

    // Create system tray icon and notification
    createTrayIcon();

    // Create tabs
    overviewPage = new OverviewPage();

//    transactionsPage = new QWidget(this);
//    QVBoxLayout *vbox = new QVBoxLayout();
    transactionView = new TransactionView(this);
//    vbox->addWidget(transactionView);
//    transactionsPage->setLayout(vbox);
    transactionsPage = transactionView;

    addressBookPage = new AddressBookPage(AddressBookPage::ForEditing, AddressBookPage::SendingTab);

    receiveCoinsPage = new AddressBookPage(AddressBookPage::ForEditing, AddressBookPage::ReceivingTab);

    sendCoinsPage = new SendCoinsDialog(this);

    miningPage = new MiningPage(this, this);
    //miningPage->SetMiningStatus(GetBoolArg("-gen"));
    serviceMessagesPage = new ServiceMessagesPage(this);
    signMessagePage = new SignMessagePage(this);
    verifyMessagePage = new VerifyMessagePage(this);
    optionsDialog = new OptionsDialog(this);

    // centralWidget = new QStackedWidget(this);
    centralWidget = ui->stackedWidget;
    centralWidget->addWidget(overviewPage);
    centralWidget->addWidget(transactionsPage);
    centralWidget->addWidget(addressBookPage);
    centralWidget->addWidget(receiveCoinsPage);
    centralWidget->addWidget(sendCoinsPage);
    centralWidget->addWidget(miningPage);
    centralWidget->addWidget(serviceMessagesPage);
    centralWidget->addWidget(signMessagePage);
    centralWidget->addWidget(verifyMessagePage);
    centralWidget->addWidget(optionsDialog);

    QSizeGrip* grip = new QSizeGrip(this);
    grip->setStyleSheet("width: 6px; height: 6px; image: url(:/res/sizegrip.png);");
    ui->horizontalLayout_8->addWidget(grip, 0, Qt::AlignBottom | Qt::AlignRight);

    // Create status bar
    //$
    // statusBar();

    // Status bar notification icons
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setContentsMargins(0,0,0,0);
    frameBlocks->setMinimumWidth(56);
    frameBlocks->setMaximumWidth(56);
    QHBoxLayout *frameBlocksLayout = new QHBoxLayout(frameBlocks);
    frameBlocksLayout->setContentsMargins(3,0,3,0);
    frameBlocksLayout->setSpacing(3);
    labelEncryptionIcon = new QLabel();
	labelKimotoGravityWellIcon = ui->label_kimotogravitywell;
    // labelConnectionsIcon = new QLabel();
    labelConnectionsIcon = ui->label_14;
    // labelBlocksIcon = new QLabel();
    labelBlocksIcon = ui->label_15;
	
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelEncryptionIcon);
    frameBlocksLayout->addStretch();
//    frameBlocksLayout->addWidget(labelConnectionsIcon);
//    frameBlocksLayout->addStretch();
//    frameBlocksLayout->addWidget(labelBlocksIcon);
//    frameBlocksLayout->addStretch();

    // Progress bar and label for blocks download
    // progressBarLabel = new QLabel();
    progressBarLabel = ui->label_12;
    progressBarLabel->setVisible(false);
    ui->label_13->setVisible(false);
    // progressBar = new QProgressBar();
    progressBar = ui->progressBar;
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setVisible(false);

    // Override style sheet for progress bar for styles that have a segmented progress bar,
    // as they make the text unreadable (workaround for issue #1071)
    // See https://qt-project.org/doc/qt-4.8/gallery.html
    QString curStyle = QApplication::style()->metaObject()->className();
    if(curStyle == "QWindowsStyle" || curStyle == "QWindowsXPStyle")
    {
        // progressBar->setStyleSheet("QProgressBar { background-color: #e8e8e8; border: 1px solid grey; border-radius: 7px; padding: 1px; text-align: center; } QProgressBar::chunk { background: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #FF8000, stop: 1 orange); border-radius: 7px; margin: 0px; }");
    }

    //$
    // statusBar()->addWidget(progressBarLabel);
    // statusBar()->addWidget(progressBar);
    // statusBar()->addPermanentWidget(frameBlocks);

    syncIconMovie = new QMovie(":/movies/update_spinner", "mng", this);

    // Clicking on a transaction on the overview page simply sends you to transaction history page
    connect(overviewPage, SIGNAL(transactionClicked(QModelIndex)), this, SLOT(gotoHistoryPage()));
    connect(overviewPage, SIGNAL(transactionClicked(QModelIndex)), transactionView, SLOT(focusTransaction(QModelIndex)));

    // Double-clicking on a transaction on the transaction history page shows details
    connect(transactionView, SIGNAL(doubleClicked(QModelIndex)), transactionView, SLOT(showDetails()));

    rpcConsole = new RPCConsole(this);
    connect(openRPCConsoleAction, SIGNAL(triggered()), rpcConsole, SLOT(show()));

    // Clicking on "Verify Message" in the address book sends you to the verify message tab
    connect(addressBookPage, SIGNAL(verifyMessage(QString)), this, SLOT(gotoVerifyMessageTab(QString)));
    // Clicking on "Sign Message" in the receive coins page sends you to the sign message tab
    connect(receiveCoinsPage, SIGNAL(signMessage(QString)), this, SLOT(gotoSignMessageTab(QString)));

    // Install event filter to be able to catch status tip events (QEvent::StatusTip)
    this->installEventFilter(this);
    ui->wMining->installEventFilter(this);
    ui->wHome->installEventFilter(this);
    ui->checkBox->installEventFilter(this);
    gotoOverviewPage();
}

void MegacoinGUI::RunMiningAsStartup()
{
    OptionsModel * opModel = clientModel->getOptionsModel();
    bool fGenerate = opModel->getStartMiningAtStartup();
    mapArgs["-gen"] = (fGenerate ? "1" : "0");
    GenerateMegacoins(fGenerate, pwalletMain);
    ui->checkBox->setChecked(fGenerate);
    miningPage->SetMiningStatus(GetBoolArg("-gen", false));
    return;
}

void MegacoinGUI::SetMiningStatus(bool isMining)
{
    if(ui->checkBox)
        ui->checkBox->setChecked(isMining);
}

MegacoinGUI::~MegacoinGUI()
{
    //saveWindowGeometry();
    if(trayIcon) // Hide tray icon, as deleting will let it linger until quit (on Ubuntu)
        trayIcon->hide();
#ifdef Q_OS_MAC
    delete appMenuBar;
#endif
    delete ui;
}

void MegacoinGUI::createActions(bool fIsTestnet)
{
    QActionGroup *tabGroup = new QActionGroup(this);

    overviewAction = new QAction(QIcon(":/icons/overview"), tr("&Overview"), this);
    overviewAction->setStatusTip(tr("Show general overview of wallet"));
    overviewAction->setToolTip(overviewAction->statusTip());
    overviewAction->setCheckable(true);
    overviewAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    tabGroup->addAction(overviewAction);

    sendCoinsAction = new QAction(QIcon(":/icons/send"), tr("&Send coins"), this);
    sendCoinsAction->setStatusTip(tr("Send coins to a Megacoin address"));
    sendCoinsAction->setToolTip(sendCoinsAction->statusTip());
    sendCoinsAction->setCheckable(true);
    sendCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
    tabGroup->addAction(sendCoinsAction);

    receiveCoinsAction = new QAction(QIcon(":/icons/receiving_addresses"), tr("&Receive coins"), this);
    receiveCoinsAction->setStatusTip(tr("Show the list of addresses for receiving payments"));
    receiveCoinsAction->setToolTip(receiveCoinsAction->statusTip());
    receiveCoinsAction->setCheckable(true);
    receiveCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));
    tabGroup->addAction(receiveCoinsAction);

    historyAction = new QAction(QIcon(":/icons/history"), tr("&Transactions"), this);
    historyAction->setStatusTip(tr("Browse transaction history"));
    historyAction->setToolTip(historyAction->statusTip());
    historyAction->setCheckable(true);
    historyAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_4));
    tabGroup->addAction(historyAction);

    addressBookAction = new QAction(QIcon(":/icons/address-book"), tr("&Address Book"), this);
    addressBookAction->setStatusTip(tr("Edit the list of stored addresses and labels"));
    addressBookAction->setToolTip(addressBookAction->statusTip());
    addressBookAction->setCheckable(true);
    addressBookAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_5));
    tabGroup->addAction(addressBookAction);

    connect(overviewAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(overviewAction, SIGNAL(triggered()), this, SLOT(gotoOverviewPage()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(gotoSendCoinsPage()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(gotoReceiveCoinsPage()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(historyAction, SIGNAL(triggered()), this, SLOT(gotoHistoryPage()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(addressBookAction, SIGNAL(triggered()), this, SLOT(gotoAddressBookPage()));
    connect(ui->bNewMessage, SIGNAL(clicked()), this, SLOT(showNormalIfMinimized()));
    connect(ui->bNewMessage, SIGNAL(clicked()), this, SLOT(gotoServiceMessagesPage()));

    quitAction = new QAction(QIcon(":/icons/quit"), tr("E&xit"), this);
    quitAction->setStatusTip(tr("Quit application"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAction->setMenuRole(QAction::QuitRole);
    if (!fIsTestnet)
        aboutAction = new QAction(QIcon(":/icons/megacoin"), tr("&About Megacoin"), this);
    else
        aboutAction = new QAction(QIcon(":/icons/megacoin_testnet"), tr("&About Megacoin"), this);
    aboutAction->setStatusTip(tr("Show information about Megacoin"));
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutQtAction = new QAction(QIcon(":/trolltech/qmessagebox/images/qtlogo-64.png"), tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show information about Qt"));
    aboutQtAction->setMenuRole(QAction::AboutQtRole);
    optionsAction = new QAction(QIcon(":/icons/options"), tr("&Options..."), this);
    optionsAction->setStatusTip(tr("Modify configuration options for Megacoin"));
    optionsAction->setMenuRole(QAction::PreferencesRole);
    if (!fIsTestnet)
        toggleHideAction = new QAction(QIcon(":/icons/megacoin"), tr("&Show / Hide"), this);
    else
        toggleHideAction = new QAction(QIcon(":/icons/megacoin_testnet"), tr("&Show / Hide"), this);
    toggleHideAction->setStatusTip(tr("Show or hide the main Window"));

    encryptWalletAction = new QAction(QIcon(":/icons/lock_closed"), tr("&Encrypt Wallet..."), this);
    encryptWalletAction->setStatusTip(tr("Encrypt the private keys that belong to your wallet"));
    encryptWalletAction->setCheckable(true);
    backupWalletAction = new QAction(QIcon(":/icons/filesave"), tr("&Backup Wallet..."), this);
    backupWalletAction->setStatusTip(tr("Backup wallet to another location"));
    changePassphraseAction = new QAction(QIcon(":/icons/key"), tr("&Change Passphrase..."), this);
    changePassphraseAction->setStatusTip(tr("Change the passphrase used for wallet encryption"));
    signMessageAction = new QAction(QIcon(":/icons/edit"), tr("Sign &message..."), this);
    signMessageAction->setStatusTip(tr("Sign messages with your Megacoin addresses to prove you own them"));
    verifyMessageAction = new QAction(QIcon(":/icons/transaction_0"), tr("&Verify message..."), this);
    verifyMessageAction->setStatusTip(tr("Verify messages to ensure they were signed with specified Megacoin addresses"));

    exportAction = new QAction(QIcon(":/icons/export"), tr("&Export..."), this);
    exportAction->setStatusTip(tr("Export the data in the current tab to a file"));
    exportAction->setToolTip(exportAction->statusTip());
    openRPCConsoleAction = new QAction(QIcon(":/icons/debugwindow"), tr("&Debug window"), this);
    openRPCConsoleAction->setStatusTip(tr("Open debugging and diagnostic console"));

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClicked()));    
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(optionsClicked()));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    connect(encryptWalletAction, SIGNAL(triggered(bool)), this, SLOT(encryptWallet(bool)));
    connect(backupWalletAction, SIGNAL(triggered()), this, SLOT(backupWallet()));
    connect(changePassphraseAction, SIGNAL(triggered()), this, SLOT(changePassphrase()));
    connect(signMessageAction, SIGNAL(triggered()), this, SLOT(gotoSignMessageTab()));
    connect(verifyMessageAction, SIGNAL(triggered()), this, SLOT(gotoVerifyMessageTab()));
}

void MegacoinGUI::createMenuBar()
{
#ifdef Q_OS_MAC
    // Create a decoupled menu bar on Mac which stays even if the window is closed
    appMenuBar = new QMenuBar();
#else
    // Get the main window's menu bar on other platforms
    appMenuBar = menuBar();
#endif

    // Configure the menus
    QMenu *file = appMenuBar->addMenu(tr("&File"));
    file->addAction(backupWalletAction);
    file->addAction(exportAction);
    file->addAction(signMessageAction);
    file->addAction(verifyMessageAction);
    file->addSeparator();
    file->addAction(quitAction);

    QMenu *settings = appMenuBar->addMenu(tr("&Settings"));
    settings->addAction(encryptWalletAction);
    settings->addAction(changePassphraseAction);
    settings->addSeparator();
    settings->addAction(optionsAction);

    QMenu *help = appMenuBar->addMenu(tr("&Help"));
    help->addAction(openRPCConsoleAction);
    help->addSeparator();
    help->addAction(aboutAction);
    help->addAction(aboutQtAction);
}

void MegacoinGUI::createToolBars()
{
    QToolBar *toolbar = addToolBar(tr("Tabs toolbar"));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->addAction(overviewAction);
    toolbar->addAction(sendCoinsAction);
    toolbar->addAction(receiveCoinsAction);
    toolbar->addAction(historyAction);
    toolbar->addAction(addressBookAction);

    QToolBar *toolbar2 = addToolBar(tr("Actions toolbar"));
    toolbar2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar2->addAction(exportAction);
}

void MegacoinGUI::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    if(clientModel)
    {
        // Create system tray menu (or setup the dock menu) that late to prevent users from calling actions,
        // while the client has not yet fully loaded
        createTrayIconMenu();

        // Keep up to date with client
        setNumConnections(clientModel->getNumConnections());
        connect(clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(setNumConnections(int)));

        setNumBlocks(clientModel->getNumBlocks(), clientModel->getNumBlocksOfPeers());
        connect(clientModel, SIGNAL(numBlocksChanged(int,int)), this, SLOT(setNumBlocks(int,int)));

        // Receive and report messages from network/worker thread
        connect(clientModel, SIGNAL(message(QString,QString,unsigned int)), this, SLOT(message(QString,QString,unsigned int)));

        overviewPage->setClientModel(clientModel);
        rpcConsole->setClientModel(clientModel);
        addressBookPage->setOptionsModel(clientModel->getOptionsModel());
        receiveCoinsPage->setOptionsModel(clientModel->getOptionsModel());
        optionsDialog->setModel(clientModel->getOptionsModel());
    }
}

void MegacoinGUI::setWalletModel(WalletModel *walletModel)
{
    this->walletModel = walletModel;
    if(walletModel)
    {
        // Receive and report messages from wallet thread
        connect(walletModel, SIGNAL(message(QString,QString,unsigned int)), this, SLOT(message(QString,QString,unsigned int)));

        // Put transaction list in tabs
        transactionView->setModel(walletModel);
        overviewPage->setWalletModel(walletModel);
        addressBookPage->setModel(walletModel->getAddressTableModel(), false);
        receiveCoinsPage->setModel(walletModel->getAddressTableModel(), false);
        sendCoinsPage->setModel(walletModel);
        signMessagePage->setModel(walletModel);
        verifyMessagePage->setModel(walletModel);

        setEncryptionStatus(walletModel->getEncryptionStatus());
        connect(walletModel, SIGNAL(encryptionStatusChanged(int)), this, SLOT(setEncryptionStatus(int)));

        // Balloon pop-up for new transaction
        connect(walletModel->getTransactionTableModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(incomingTransaction(QModelIndex,int,int)));

        // Ask for passphrase if needed
        connect(walletModel, SIGNAL(requireUnlock()), this, SLOT(unlockWallet()));
    }
}

void MegacoinGUI::createTrayIcon()
{
#ifndef Q_OS_MAC
    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setToolTip(tr("Megacoin client"));
    trayIcon->setIcon(QIcon(":/icons/toolbar"));
    trayIcon->show();
#endif

    notificator = new Notificator(QApplication::applicationName(), trayIcon);
}

void MegacoinGUI::createTrayIconMenu()
{
    QMenu *trayIconMenu;
#ifndef Q_OS_MAC
    // return if trayIcon is unset (only on non-Mac OSes)
    if (!trayIcon)
        return;

    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
#else
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler *dockIconHandler = MacDockIconHandler::instance();
    dockIconHandler->setMainWindow((QMainWindow *)this);
    trayIconMenu = dockIconHandler->dockMenu();
#endif

    // Configuration of the tray icon (or dock icon) icon menu
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(sendCoinsAction);
    trayIconMenu->addAction(receiveCoinsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(signMessageAction);
    trayIconMenu->addAction(verifyMessageAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(optionsAction);
    // trayIconMenu->addAction(openRPCConsoleAction);
#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
#endif
}

#ifndef Q_OS_MAC
void MegacoinGUI::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        // Click on system tray icon triggers show/hide of the main window
        toggleHideAction->trigger();
    }
}
#endif

void MegacoinGUI::saveWindowGeometry()
{
    QSettings settings;
    settings.setValue("nWindowPos", pos());
    settings.setValue("nWindowSize", size());
}

void MegacoinGUI::restoreWindowGeometry()
{
    QSettings settings;
    QPoint pos = settings.value("nWindowPos").toPoint();
    QSize size = settings.value("nWindowSize", QSize(850, 550)).toSize();
    if (!pos.x() && !pos.y())
    {
        QRect screen = QApplication::desktop()->screenGeometry();
        pos.setX((screen.width()-size.width())/2);
        pos.setY((screen.height()-size.height())/2);
    }
    resize(size);
    move(pos);
}

void MegacoinGUI::optionsClicked()
{
    if(!clientModel || !clientModel->getOptionsModel())
        return;

    centralWidget->setCurrentWidget(optionsDialog);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
}

void MegacoinGUI::aboutClicked()
{
    AboutDialog dlg(this);
    dlg.setModel(clientModel);
    dlg.exec();
}

void MegacoinGUI::setNumConnections(int count)
{
    QString icon;
    switch(count)
    {
    case 0: icon = ":/res/connection_1.png"; break;
    case 1: case 2: icon = ":/res/connection_2.png"; break;
    case 3: case 4: icon = ":/res/connection_3.png"; break;
    case 5: case 6: icon = ":/res/connection_4.png"; break;
    case 7: case 8: case 9: icon = ":/res/connection_5.png"; break;
    default: icon = ":/res/connection_5.png"; break;
    }
    labelConnectionsIcon->setPixmap(QPixmap(icon));
    labelConnectionsIcon->setToolTip(tr("%n active connection(s) to Megacoin network", "", count));
}

void MegacoinGUI::setNumBlocks(int count, int nTotalBlocks)
{
    // Prevent orphan statusbar messages (e.g. hover Quit in main menu, wait until chain-sync starts -> garbelled text)
    //$
    // statusBar()->clearMessage();

    // don't show / hide progress bar and its label if we have no connection to the network
    enum BlockSource blockSource = clientModel ? clientModel->getBlockSource() : BLOCK_SOURCE_NONE;
    if (blockSource == BLOCK_SOURCE_NONE || (blockSource == BLOCK_SOURCE_NETWORK && clientModel->getNumConnections() == 0))
    {
        progressBarLabel->setVisible(false);
        progressBar->setVisible(false);
        ui->label_13->setVisible(false);

        return;
    }

    QString tooltip;

    QString importText;
    switch (blockSource) {
    case BLOCK_SOURCE_NONE:
    case BLOCK_SOURCE_NETWORK:
        importText = tr("Synchronizing with network...");
        break;
    case BLOCK_SOURCE_DISK:
        importText = tr("Importing blocks from disk...");
        break;
    case BLOCK_SOURCE_REINDEX:
        importText = tr("Reindexing blocks on disk...");
    }

    if(count < nTotalBlocks)
    {
        int nRemainingBlocks = nTotalBlocks - count;
        float nPercentageDone = count / (nTotalBlocks * 0.01f);

        progressBarLabel->setText(importText);
        progressBarLabel->setVisible(true);
        progressBar->setFormat(tr("~%n block(s) remaining", "", nRemainingBlocks));
        progressBar->setMaximum(nTotalBlocks);
        progressBar->setValue(count);
        progressBar->setVisible(true);
        ui->label_13->setText(tr("%n blocks", "", count));
        ui->label_13->setVisible(true);

        tooltip = tr("Processed %1 of %2 blocks of transaction history (%3% done).").arg(count).arg(nTotalBlocks).arg(nPercentageDone, 0, 'f', 2);
    }
    else
    {
        progressBarLabel->setVisible(false);

        progressBar->setVisible(false);
        ui->label_13->setVisible(false);
        tooltip = tr("Processed %1 blocks of transaction history.").arg(count);
    }

    QDateTime lastBlockDate = clientModel->getLastBlockDate();
    int secs = lastBlockDate.secsTo(QDateTime::currentDateTime());
    QString text;

    // Represent time from last generated block in human readable text
    if(secs <= 0)
    {
        // Fully up to date. Leave text empty.
    }
    else if(secs < 60)
    {
        text = tr("%n second(s) ago","",secs);
    }
    else if(secs < 60*60)
    {
        text = tr("%n minute(s) ago","",secs/60);
    }
    else if(secs < 24*60*60)
    {
        text = tr("%n hour(s) ago","",secs/(60*60));
    }
    else
    {
        text = tr("%n day(s) ago","",secs/(60*60*24));
    }

    // Set icon state: spinning if catching up, tick otherwise
    if(secs < 90*60 && count >= nTotalBlocks)
    {
        tooltip = tr("Up to date") + QString(".<br>") + tooltip;
        labelBlocksIcon->setPixmap(QPixmap(":/res/sync_4.png"));

        overviewPage->showOutOfSyncWarning(false);
    }
    else
    {
        tooltip = tr("Catching up...") + QString("<br>") + tooltip;
        // labelBlocksIcon->setMovie(syncIconMovie);
        if (count < nTotalBlocks / 3)
            labelBlocksIcon->setPixmap(QPixmap(":/res/sync_1.png"));
        else if (count < 2 * nTotalBlocks / 3)
        labelBlocksIcon->setPixmap(QPixmap(":/res/sync_2.png"));
        else
            labelBlocksIcon->setPixmap(QPixmap(":/res/sync_3.png"));
        // syncIconMovie->start();

        overviewPage->showOutOfSyncWarning(true);
    }
	
	if (nTotalBlocks >= 62773) {
		labelKimotoGravityWellIcon->setPixmap(QPixmap(":/res/kimotogravitywell_icon_on.png"));
	}

    if(!text.isEmpty())
    {
        tooltip += QString("<br>");
        tooltip += tr("Last received block was generated %1.").arg(text);
    }

    // Don't word-wrap this (fixed-width) tooltip
    tooltip = QString("<nobr>") + tooltip + QString("</nobr>");

    labelBlocksIcon->setToolTip(tooltip);
    progressBarLabel->setToolTip(tooltip);
    progressBar->setToolTip(tooltip);
}

void MegacoinGUI::message(const QString &title, const QString &message, unsigned int style, bool *ret)
{
    QString strTitle = tr("Megacoin"); // default title
    // Default to information icon
    int nMBoxIcon = QMessageBox::Information;
    int nNotifyIcon = Notificator::Information;

    // Override title based on style
    QString msgType;
    switch (style) {
    case CClientUIInterface::MSG_ERROR:
        msgType = tr("Error");
        break;
    case CClientUIInterface::MSG_WARNING:
        msgType = tr("Warning");
        break;
    case CClientUIInterface::MSG_INFORMATION:
        msgType = tr("Information");
        break;
    default:
        msgType = title; // Use supplied title
    }
    if (!msgType.isEmpty())
        strTitle += " - " + msgType;

    // Check for error/warning icon
    if (style & CClientUIInterface::ICON_ERROR) {
        nMBoxIcon = QMessageBox::Critical;
        nNotifyIcon = Notificator::Critical;
    }
    else if (style & CClientUIInterface::ICON_WARNING) {
        nMBoxIcon = QMessageBox::Warning;
        nNotifyIcon = Notificator::Warning;
    }

    // Display message
    if (style & CClientUIInterface::MODAL) {
        // Check for buttons, use OK as default, if none was supplied
        QMessageBox::StandardButton buttons;
        if (!(buttons = (QMessageBox::StandardButton)(style & CClientUIInterface::BTN_MASK)))
            buttons = QMessageBox::Ok;

        QMessageBox mBox((QMessageBox::Icon)nMBoxIcon, strTitle, message, buttons);
        int r = mBox.exec();
        if (ret != NULL)
            *ret = r == QMessageBox::Ok;
    }
    else
        notificator->notify((Notificator::Class)nNotifyIcon, strTitle, message);
}

void MegacoinGUI::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
#ifndef Q_OS_MAC // Ignored on Mac
    if(e->type() == QEvent::WindowStateChange)
    {
        if(clientModel && clientModel->getOptionsModel()->getMinimizeToTray())
        {
            QWindowStateChangeEvent *wsevt = static_cast<QWindowStateChangeEvent*>(e);
            if(!(wsevt->oldState() & Qt::WindowMinimized) && isMinimized())
            {
                QTimer::singleShot(0, this, SLOT(hide()));
                e->ignore();
            }
        }
    }
#endif
}

void MegacoinGUI::closeEvent(QCloseEvent *event)
{
    if(clientModel)
    {
#ifndef Q_OS_MAC // Ignored on Mac
        if(!clientModel->getOptionsModel()->getMinimizeToTray() &&
           !clientModel->getOptionsModel()->getMinimizeOnClose())
        {
            QApplication::quit();
        }
#endif
    }
    QMainWindow::closeEvent(event);
}

void MegacoinGUI::askFee(qint64 nFeeRequired, bool *payFee)
{
    QString strMessage = tr("This transaction is over the size limit. You can still send it for a fee of %1, "
        "which goes to the nodes that process your transaction and helps to support the network. "
        "Do you want to pay the fee?").arg(MegacoinUnits::formatWithUnit(MegacoinUnits::MEC, nFeeRequired));
    QMessageBox::StandardButton retval = QMessageBox::question(
          this, tr("Confirm transaction fee"), strMessage,
          QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    *payFee = (retval == QMessageBox::Yes);
}

//void MegacoinGUI::incomingTransaction(const QString& date, int unit, qint64 amount, const QString& type, const QString& address)
void MegacoinGUI::incomingTransaction(const QModelIndex& parent, int start, int /*end*/)
{
    // Prevent balloon-spam when initial block download is in progress
    if(!walletModel || !clientModel || clientModel->inInitialBlockDownload())
        return;

    TransactionTableModel *ttm = walletModel->getTransactionTableModel();

    QString date = ttm->index(start, TransactionTableModel::Date, parent)
                     .data().toString();
    qint64 amount = ttm->index(start, TransactionTableModel::Amount, parent)
                      .data(Qt::EditRole).toULongLong();
    QString type = ttm->index(start, TransactionTableModel::Type, parent)
                     .data().toString();
    QString address = ttm->index(start, TransactionTableModel::ToAddress, parent)
                        .data().toString();

    // Play sound
    if (clientModel->getOptionsModel()->getAllowSounds())
    {
        // Incomings only
        if (amount > 0)
        {
            boost::filesystem::path wavPath = GetDataDir() / "incom.wav";
            QString strWavPath = QString::fromStdString(wavPath.string());
            if (!QFile::exists(strWavPath))
                QFile::copy(":/res/incom.wav", strWavPath);
            QSound::play(strWavPath);
        }
    }

    // On new transaction, make an info balloon
    message((amount)<0 ? tr("Sent transaction") : tr("Incoming transaction"),
             tr("Date: %1\n"
                "Amount: %2\n"
                "Type: %3\n"
                "Address: %4\n")
                  .arg(date)
                  .arg(MegacoinUnits::formatWithUnit(walletModel->getOptionsModel()->getDisplayUnit(), amount, true))
                  .arg(type)
                  .arg(address), CClientUIInterface::MSG_INFORMATION);
}

void MegacoinGUI::gotoOverviewPage()
{
    overviewAction->setChecked(true);
    centralWidget->setCurrentWidget(overviewPage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
}

void MegacoinGUI::gotoHistoryPage()
{
    historyAction->setChecked(true);
    centralWidget->setCurrentWidget(transactionsPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), transactionView, SLOT(exportClicked()));
}

void MegacoinGUI::gotoAddressBookPage()
{
    addressBookAction->setChecked(true);
    centralWidget->setCurrentWidget(addressBookPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), addressBookPage, SLOT(exportClicked()));
}

void MegacoinGUI::gotoReceiveCoinsPage()
{
    receiveCoinsAction->setChecked(true);
    centralWidget->setCurrentWidget(receiveCoinsPage);

    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), receiveCoinsPage, SLOT(exportClicked()));
}

//void MegacoinGUI::gotoSendCoinsPage(QString addr)
void MegacoinGUI::gotoSendCoinsPage()
{
    sendCoinsAction->setChecked(true);
    centralWidget->setCurrentWidget(sendCoinsPage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    
    //if(!addr.isEmpty())
    //sendCoinsPage->setAddress(addr);
}

void MegacoinGUI::gotoSignMessageTab(QString addr)
{
    centralWidget->setCurrentWidget(signMessagePage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);

    // call show() in showTab_SM()
    signMessagePage->showTab_SM(true);

    if(!addr.isEmpty())
        signMessagePage->setAddress_SM(addr);
}

void MegacoinGUI::gotoVerifyMessageTab(QString addr)
{
    centralWidget->setCurrentWidget(verifyMessagePage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);

    // call show() in showTab_VM()
    verifyMessagePage->showTab_VM(true);

    if(!addr.isEmpty())
        verifyMessagePage->setAddress_VM(addr);
}

void MegacoinGUI::gotoServiceMessagesPage()
{
    centralWidget->setCurrentWidget(serviceMessagesPage);

    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
}

void MegacoinGUI::dragEnterEvent(QDragEnterEvent *event)
{
    // Accept only URIs
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MegacoinGUI::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        int nValidUrisFound = 0;
        QList<QUrl> uris = event->mimeData()->urls();
        foreach(const QUrl &uri, uris)
        {
            //if (walletFrame->handleURI(uri.toString()))
	    if (sendCoinsPage->handleURI(uri.toString()))
                nValidUrisFound++;
        }

        // if valid URIs were found
        if (nValidUrisFound)
            //walletFrame->gotoSendCoinsPage();
	    gotoSendCoinsPage();
        else
            message(tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid Megacoin address or malformed URI parameters."),
                      CClientUIInterface::ICON_WARNING);
    }

    event->acceptProposedAction();
}

bool MegacoinGUI::eventFilter(QObject *object, QEvent *event)
{
    // Catch status tip events
    if (event->type() == QEvent::StatusTip)
    {
        // Prevent adding text from setStatusTip(), if we currently use the status bar for displaying other stuff
        if (progressBarLabel->isVisible() || progressBar->isVisible())
            return true;
    }
    if (object == ui->wMining && event->type() == QEvent::MouseButtonPress)
        onMiningClicked();
    if (object == ui->wHome && event->type() == QEvent::MouseButtonPress)
        gotoOverviewPage();
    if (object == ui->checkBox && event->type() == QEvent::MouseButtonPress)
        onMiningClicked();
    return QMainWindow::eventFilter(object, event);
}

void MegacoinGUI::resizeEvent(QResizeEvent *e)
{
    updateMask();
    QMainWindow::resizeEvent(e);
}

void MegacoinGUI::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    updateMask();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // we need this in order to get correct rounded corners
    painter.setPen(QPen(QBrush(Qt::black), 2.0));
    painter.setBrush(QBrush(QColor(140, 30, 37)));

    QMainWindow::paintEvent(e);
}

void MegacoinGUI::updateMask()
{
    // main form mask
    _mask = QBitmap(size());
    _mask.clear();
    {
        QPainter painter(&_mask);
        painter.fillRect(rect(), Qt::color1);
        painter.drawImage(0, 0, QImage("://res/mask0.png"));
        painter.end();
    }
    setMask(_mask);

    // logo widget corner
    _logoWidgetMask = QBitmap(ui->wHeader->size());
    _logoWidgetMask.clear();

    {
        QPainter painter(&_logoWidgetMask);
        painter.fillRect(ui->wHeader->rect(), Qt::color1);
        painter.drawImage(0, 0, QImage("://res/mask1.png"));
        painter.end();
    }
    ui->wHeader->setMask(_logoWidgetMask);
}

void MegacoinGUI::handleURI(QString strURI)
{
    // URI has to be valid
    if (sendCoinsPage->handleURI(strURI))
    {
        showNormalIfMinimized();
        gotoSendCoinsPage();
    }
    else
        message(tr("URI handling"), tr("URI can not be parsed! This can be caused by an invalid Megacoin address or malformed URI parameters."),
                  CClientUIInterface::ICON_WARNING);
}

void MegacoinGUI::menuFileRequested()
{
    QMenu menu(this);
    QAction* home = menu.addAction(tr("&Overview").remove('&'));
    QAction* messages = menu.addAction(tr("Service messages"));
    QAction* qaBackupWallet = menu.addAction(tr("Backup Wallet"));
    QAction* exportData = menu.addAction(tr("&Export...").remove('&').remove("..."));
    QAction* exitApp = menu.addAction(QIcon("://res/menu/menu_exit.png"), tr("E&xit").remove('&'));

    QPoint poz = QCursor::pos();
    if (QWidget* w = qobject_cast<QWidget*>(sender()))
        poz = w->mapToGlobal(w->rect().bottomLeft());

    QAction* selected = menu.exec(poz);
    if (!selected)
        return;

    if (selected == home)
    {
        gotoOverviewPage();
    }
    else if (selected == messages)
    {
        gotoServiceMessagesPage();
    }
    else if (selected == qaBackupWallet) {
        this->backupWallet();
    }
    else if (selected == exportData)
    {
        if (centralWidget->currentWidget() == transactionsPage)
        {
            transactionView->exportClicked();
        } else if (centralWidget->currentWidget() == addressBookPage)
        {
            addressBookPage->exportClicked();
        } else if (centralWidget->currentWidget() == receiveCoinsPage)
        {
            receiveCoinsPage->exportClicked();
        }
    }
    else if (selected == exitApp)
    {
        qApp->quit();
    }
}

void MegacoinGUI::menuOperationsRequested()
{
    QMenu menu(this);
    QAction* send = menu.addAction(tr("Send Megacoins"));
    QAction* receive = menu.addAction(tr("Receive Megacoins"));
    QAction* transactions = menu.addAction(tr("&Transactions").remove('&'));
    QAction* addressBook = menu.addAction(tr("&Address Book").remove('&'));
    QAction* encryptWallet = menu.addAction(tr("&Encrypt Wallet...").remove('&').remove("..."));
    QAction* mining = menu.addAction(tr("Mining"));
    QAction* changePassword = menu.addAction(tr("&Change Passphrase...").remove('&').remove("..."));
    QAction* signMessage = menu.addAction(QIcon("://res/menu/sign.png"), tr("Sign &message...").remove('&').remove("..."));
    QAction* verifySignature = menu.addAction(QIcon("://res/menu/check_signature.png"), tr("&Verify message...").remove('&').remove("..."));

    QPoint poz = QCursor::pos();
    if (QWidget* w = qobject_cast<QWidget*>(sender()))
        poz = w->mapToGlobal(w->rect().bottomLeft());

    QAction* selected = menu.exec(poz);
    if (!selected)
        return;

    if (selected == send)
    {
        gotoSendCoinsPage();
    }
    else if (selected == receive)
    {
        gotoReceiveCoinsPage();
    }
    else if (selected == transactions)
    {
        gotoHistoryPage();
    }
    else if (selected == addressBook)
    {
        gotoAddressBookPage();
    }
    else if (selected == encryptWallet)
    {
        if (walletModel->getEncryptionStatus() == WalletModel::Unencrypted)
            this->encryptWallet(true);
    }
    else if (selected == mining)
    {
        onMiningClicked();
    }
    else if (selected == changePassword)
    {
        if (walletModel->getEncryptionStatus() == WalletModel::Unlocked || walletModel->getEncryptionStatus() == WalletModel::Locked)
            changePassphrase();
    }
    else if (selected == signMessage)
    {
        gotoSignMessageTab();
    }
    else if (selected == verifySignature)
    {
        gotoVerifyMessageTab();
    }
}

void MegacoinGUI::menuSettingsRequested()
{
    QMenu menu(this);
    QAction* commonAndNetwork = menu.addAction(tr("Common, Network"));
    QAction* debugWindow = menu.addAction(tr("&Debug window").remove('&'));
    QAction* about = menu.addAction(tr("&About Megacoin").remove('&'));    

    QPoint poz = QCursor::pos();
    if (QWidget* w = qobject_cast<QWidget*>(sender()))
        poz = w->mapToGlobal(w->rect().bottomLeft());

    QAction* selected = menu.exec(poz);
    if (!selected)
        return;

    if (selected == commonAndNetwork)
    {
        optionsClicked();
    }
    else if (selected == about)
    {
        aboutClicked();
    } else if (selected == debugWindow)
    {
        on_bHelp_clicked();
    }
}

void MegacoinGUI::setEncryptionStatus(int status)
{
    switch(status)
    {
    case WalletModel::Unencrypted:
        labelEncryptionIcon->hide();
        encryptWalletAction->setChecked(false);
        changePassphraseAction->setEnabled(false);
        encryptWalletAction->setEnabled(true);
        break;
    case WalletModel::Unlocked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_open").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>unlocked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        break;
    case WalletModel::Locked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_closed").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>locked</b>"));
        encryptWalletAction->setChecked(true);
        changePassphraseAction->setEnabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        break;
    }
}

void MegacoinGUI::encryptWallet(bool status)
{
    if(!walletModel)
        return;
    AskPassphraseDialog dlg(status ? AskPassphraseDialog::Encrypt:
                                     AskPassphraseDialog::Decrypt, this);
    dlg.setModel(walletModel);
    dlg.exec();

    setEncryptionStatus(walletModel->getEncryptionStatus());
}

void MegacoinGUI::backupWallet()
{
    QString saveDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString filename = QFileDialog::getSaveFileName(this, tr("Backup Wallet"), saveDir, tr("Wallet Data (*.dat)"));
    if(!filename.isEmpty()) {
        if(!walletModel->backupWallet(filename)) {
            message(tr("Backup Failed"), tr("There was an error trying to save the wallet data to the new location."),
                      CClientUIInterface::MSG_ERROR);
        }
        else
            message(tr("Backup Successful"), tr("The wallet data was successfully saved to the new location."),
                      CClientUIInterface::MSG_INFORMATION);
    }
}

void MegacoinGUI::changePassphrase()
{
    AskPassphraseDialog dlg(AskPassphraseDialog::ChangePass, this);
    dlg.setModel(walletModel);
    dlg.exec();
}

void MegacoinGUI::unlockWallet()
{
    if(!walletModel)
        return;
    // Unlock wallet when requested by wallet model
    if(walletModel->getEncryptionStatus() == WalletModel::Locked)
    {
        AskPassphraseDialog dlg(AskPassphraseDialog::Unlock, this);
        dlg.setModel(walletModel);
        dlg.exec();
    }
}

void MegacoinGUI::showNormalIfMinimized(bool fToggleHidden)
{
    // activateWindow() (sometimes) helps with keyboard focus on Windows
    if (isHidden())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else if (GUIUtil::isObscured(this))
    {
        raise();
        activateWindow();
    }
    else if(fToggleHidden)
        hide();
}

void MegacoinGUI::toggleHidden()
{
    showNormalIfMinimized(true);
}

void MegacoinGUI::detectShutdown()
{
    if (ShutdownRequested())
        QMetaObject::invokeMethod(QCoreApplication::instance(), "quit", Qt::QueuedConnection);
}

void MegacoinGUI::onMiningClicked()
{
    //ui->wMining->setStyleSheet("background-color:black;");
    centralWidget->setCurrentWidget(miningPage);
}

void MegacoinGUI::on_bHelp_clicked()
{    
    rpcConsole->show();
}
