#include "miningpage.h"
#include "ui_miningpage.h"
#include "guiutil.h"
#include "util.h"
#include "main.h"
#include "init.h"
#include "megacoingui.h"

#include <QTime>
#include <QTimer>
#include <QThread>
#include <QTextEdit>
#include <QKeyEvent>
#include <QUrl>
#include <QScrollBar>

MiningPage::MiningPage(QWidget *parent, MegacoinGUI *mainForm) :
    QWidget(parent),
    ui(new Ui::MiningPage)
{    
    _mainFrom = mainForm;
    ui->setupUi(this);
    clear();
    ui->procSlider->setMinimum(1);
    int nProcessors = boost::thread::hardware_concurrency();
    ui->procSlider->setMaximum(nProcessors);
    ui->procSlider->setValue(nProcessors / 2); 
    QTimer* ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(slotUpdateSpeed()));
    ptimer->start(1000);
    slotUpdateSpeed();    
}

void MiningPage::slotUpdateSpeed()
{
        boost::int64_t speed = 0;
        if (GetTimeMillis() - nHPSTimerStart > 8000)
            speed = (boost::int64_t)0;
        else
            speed = (boost::int64_t)dHashesPerSec;
        if (ui->bStopMining->isEnabled())
        ui->lSpeed->setText(QString("%1 Kh/s").arg(speed / 1000));        
}

MiningPage::~MiningPage()
{
    delete ui;
}

void MiningPage::scrollToEnd()
{
    QScrollBar *scrollbar = ui->miningLog->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}

void MiningPage::SetMiningStatus(bool isMining)
{
    ui->bStartMining->setEnabled(!isMining);
    ui->bStopMining->setEnabled(isMining);
    ui->procSlider->setEnabled(!isMining);
    if (!isMining)
        ui->lSpeed->setText("");
    _mainFrom->SetMiningStatus(isMining);
    if (isMining)
    {
        message(CMD_REQUEST, tr("Mining coins started!"));
        int64 pc = GetArg("-genproclimit", -1);
        message(CMD_REQUEST, QString(tr("Used threads %1")).arg(pc));
    }
    else
        message(CMD_REQUEST, tr("Mining coins stopped!"));
}

void MiningPage::clear()
{
    ui->miningLog->clear();

    ui->miningLog->document()->setDefaultStyleSheet(
                "table { }"
                "td.time { color: #808080; padding-top: 3px; } "
                "td.message { font-family: Monospace; font-size: 12px; } "
                "td.cmd-request { color: #590607; } "
                "td.cmd-error { color: red; } "
                "b { color: #590607; } "
                );
}

static QString categoryClass(int category)
{
    switch(category)
    {
    case MiningPage::CMD_REQUEST:  return "cmd-request"; break;
    case MiningPage::CMD_REPLY:    return "cmd-reply"; break;
    case MiningPage::CMD_ERROR:    return "cmd-error"; break;
    default:                       return "misc";
    }
}

void MiningPage::message(int category, const QString &message, bool html)
{
    QTime time = QTime::currentTime();
    QString timeString = time.toString();
    QString out;
    out += "<table><tr><td class=\"time\" width=\"65\">" + timeString + "</td>";
    out += "<td class=\"icon\" width=\"32\"><img src=\"" + categoryClass(category) + "\"></td>";
    out += "<td class=\"message " + categoryClass(category) + "\" valign=\"middle\">";
    if(html)
        out += message;
    else
        out += GUIUtil::HtmlEscape(message, true);
    out += "</td></tr></table>";
    ui->miningLog->append(out);
}

void MiningPage::on_bStartMining_clicked()
{
    message(CMD_REPLY, tr("Running mining coins..."));
    try
    {
        bool res = SetGenerate(true, ui->procSlider->value());
        if (res == true)
            SetMiningStatus(res);
        else
            message(CMD_ERROR, tr("Start error!"));
    }
    catch (std::exception& e)
    {
        message(CMD_ERROR, QString("Error: ") + QString::fromStdString(e.what()));
    }
    scrollToEnd();
}

void MiningPage::on_bStopMining_clicked()
{
    message(CMD_REPLY, tr("Stop mining coins..."));
    try
    {
        bool res = SetGenerate(false, -1);
        if (res == false)
            SetMiningStatus(res);
        else
            message(CMD_ERROR, tr("Stop error!"));
    }
    catch (std::exception& e)
    {
        message(CMD_ERROR, QString("Error: ") + QString::fromStdString(e.what()));
    }
    scrollToEnd();
}

bool MiningPage::SetGenerate(bool start, int proc)
{
    bool fGenerate = start;
    int nGenProcLimit = proc;
    mapArgs["-genproclimit"] = itostr(nGenProcLimit);
    if (nGenProcLimit == 0)
        fGenerate = false;
    mapArgs["-gen"] = (fGenerate ? "1" : "0");
    GenerateMegacoins(fGenerate, pwalletMain);
    return GetBoolArg("-gen", false);
}
