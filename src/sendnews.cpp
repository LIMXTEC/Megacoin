
#include "net.h"
#include "key.h"
#include "newsmessage.h"
#include "base58.h"
#include "sendnews.h"
#include "megacoinrpc.h"

static const int64 DAYS = 24 * 60 * 60;

CSendNewsMessage::CSendNewsMessage()
{
    return;
}

CSendNewsMessage::~CSendNewsMessage()
{
    return;
}

void CSendNewsMessage::Test(string header, string text, string trayNotify, string pKey)
{
    CNewsMessage message;
    message.nRelayUntil   = GetTime() + 15 * 60;
    message.nExpiration   = GetTime() + 1 * 120;
    message.nID           = 1;
    message.nTime         = GetTime();
    message.nPriority     = 1;
    message.strHeader = header;
    message.strMessage = text;
    message.strTrayNotify = trayNotify;

    // Sign
    string strSecret = pKey;
    CDataStream sMsg(SER_NETWORK, PROTOCOL_VERSION);
    sMsg << *(CUnsignedNewsMessage*)&message;
    message.vchMsg = std::vector<unsigned char>(sMsg.begin(), sMsg.end());

    CMegacoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);
    if (!fGood)
        throw runtime_error("Invalid private key");
    CKey key = vchSecret.GetKey();

    if (!key.Sign(Hash(message.vchMsg.begin(), message.vchMsg.end()), message.vchSig))
        throw runtime_error("ThreadSendNews() : key.Sign failed");

    // Test
    CDataStream sBuffer(SER_NETWORK, PROTOCOL_VERSION);
    sBuffer << message;
    CNewsMessage message2;
    sBuffer >> message2;
    if (!message2.CheckSignature())
        throw runtime_error("ThreadSendNews() : CheckSignature failed");
    assert(message2.vchMsg == message.vchMsg);
    assert(message2.vchSig == message.vchSig);
    message.SetNull();

    //Check
    while (vNodes.size() < 1) {
		boost::this_thread::interruption_point();
        MilliSleep(500);
	}
	boost::this_thread::interruption_point();
	
    // Send
    int nSent = 0;

    BOOST_FOREACH(CNode* pnode, vNodes)
    {
        if (message2.RelayTo(pnode))
             nSent++;
    }
}
