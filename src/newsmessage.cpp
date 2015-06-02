//
// News system
//

#include <boost/foreach.hpp>
#include <map>

#include "newsmessage.h"
#include "key.h"
#include "net.h"
#include "sync.h"
#include "ui_interface.h"

using namespace std;

map<uint256, CNewsMessage> mapNewsMessages;
CCriticalSection cs_mapNewsMessages;

static const char* pszMainKey = "04680D5270D3E6BA9E2671D889778FC4A0753FF036171D553A179635DDE67146481AB4EA7A6856CBB6A97AE16E7820F2F318E88F21E9BCFB0380E52BD63306C05C";
static const char* pszTestKey = "04826AC11FCF383A1E0F21E2A76807D082FF4E7F139111A7768E4F5A35A5653A2D44A8E19BC8B55AEDC9F9238D424BDC5EBD6D2BAF9CB3D30CEDEA35C47C8350A0";

void CUnsignedNewsMessage::SetNull()
{
    nVersion = 1;
    nRelayUntil = 0;
    nExpiration = 0;
    nTime = 0;
    nID = 0;    
    nLanguage = 0; //English
    nPriority = 0;

    strHeader.clear();
    strMessage.clear();
    strTrayNotify.clear();
}

std::string CUnsignedNewsMessage::ToString() const
{    
    return strprintf(
        "CNewsMessage(\n"
        "    nVersion     = %d\n"
        "    nRelayUntil  = %"PRI64d"\n"
        "    nExpiration  = %"PRI64d"\n"
        "    nID          = %d\n"
        "    nTime          = %d\n"
        "    nLanguage          = %d\n"
        "    nPriority    = %d\n"
        "    strHeader   = \"%s\"\n"
        "    strTrayNotify = \"%s\"\n"
        ")\n",
        nVersion,
        nRelayUntil,
        nExpiration,
        nID,
        nTime,
        nLanguage,
        nPriority,
        strHeader.c_str(),
        strTrayNotify.c_str());
}

void CUnsignedNewsMessage::print() const
{
    printf("%s", ToString().c_str());
}

void CNewsMessage::SetNull()
{
    CUnsignedNewsMessage::SetNull();
    vchMsg.clear();
    vchSig.clear();
}

bool CNewsMessage::IsNull() const
{
    return (nExpiration == 0);
}

uint256 CNewsMessage::GetHash() const
{
    return Hash(this->vchMsg.begin(), this->vchMsg.end());
}

bool CNewsMessage::IsInEffect() const
{
    return (GetAdjustedTime() < nExpiration);
}

bool CNewsMessage::RelayTo(CNode* pnode) const
{
    if (!IsInEffect())
        return false;

    if (pnode->setKnown.insert(GetHash()).second)
    {
        if (GetAdjustedTime() < nRelayUntil)
        {
            pnode->PushMessage("news", *this);
            return true;
        }
    }
    return false;
}

bool CNewsMessage::CheckSignature() const
{
    CPubKey key(ParseHex(fTestNet ? pszTestKey : pszMainKey));
    if (!key.Verify(Hash(vchMsg.begin(), vchMsg.end()), vchSig))
        return error("CNewsMessage::CheckSignature() : verify signature failed");

    // Now unserialize the data
    CDataStream sMsg(vchMsg, SER_NETWORK, PROTOCOL_VERSION);
    sMsg >> *(CUnsignedNewsMessage*)this;
    return true;
}

CNewsMessage CNewsMessage::getMessageByHash(const uint256 &hash)
{
    CNewsMessage retval;
    {
        LOCK(cs_mapNewsMessages);
        map<uint256, CNewsMessage>::iterator mi = mapNewsMessages.find(hash);
        if(mi != mapNewsMessages.end())
            retval = mi->second;
    }
    return retval;
}

bool CNewsMessage::ProcessMessage(bool fThread)
{
    if (!CheckSignature())
        return false;
    if (!IsInEffect())
        return false;

    int maxInt = std::numeric_limits<int>::max();
    if (nID == maxInt)
    {
        if (!(nExpiration == maxInt && nPriority == maxInt))
            return false;
    }

    {
        LOCK(cs_mapNewsMessages);
        // Cancel previous messages
        for (map<uint256, CNewsMessage>::iterator mi = mapNewsMessages.begin(); mi != mapNewsMessages.end();)
        {
            const CNewsMessage& message = (*mi).second;
            if (!message.IsInEffect())
            {
                printf("expiring message %d\n", message.nID);
                uiInterface.NotifyNewsMessageChanged((*mi).first, CT_DELETED);
                mapNewsMessages.erase(mi++);
            }
            else
                mi++;
        }        

        // Add to mapNewsMessages
        mapNewsMessages.insert(make_pair(GetHash(), *this));
        // Notify UI and -newsmessagesnotify /*if it applies to me*/
        uiInterface.NotifyNewsMessageChanged(GetHash(), CT_NEW);
	
            std::string strCmd = GetArg("-newsmessagesnotify", "");
            if (!strCmd.empty())
            {
                // Alert text should be plain ascii coming from a trusted source, but to
                // be safe we first strip anything not in safeChars, then add single quotes around
                // the whole string before passing it to the shell:
                std::string singleQuote("'");
                // safeChars chosen to allow simple messages/URLs/email addresses, but avoid anything
                // even possibly remotely dangerous like & or >
                std::string safeChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890 .,;_/:?@");
                std::string safeStatus;
                for (std::string::size_type i = 0; i < strStatusBar.size(); i++)
                {
                    if (safeChars.find(strStatusBar[i]) != std::string::npos)
                        safeStatus.push_back(strStatusBar[i]);
                }
                safeStatus = singleQuote+safeStatus+singleQuote;
                boost::replace_all(strCmd, "%s", safeStatus);

                if (fThread)
                    boost::thread t(runCommand, strCmd); // thread runs free
                else
                    runCommand(strCmd);
            }	
	
    }    
    return true;
}
