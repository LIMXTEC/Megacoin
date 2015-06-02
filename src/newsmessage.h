#ifndef CNEWSMESSAGE_H
#define CNEWSMESSAGE_H

#include <set>
#include <string>

#include "uint256.h"
#include "util.h"

class CNode;

class CUnsignedNewsMessage
{
public:
    int nVersion;
    int64 nRelayUntil;
    int64 nExpiration;
    int nID;
    int nTime;
    int nLanguage;
    int nPriority;

    // Messages
    std::string strHeader;
    std::string strMessage;
    std::string strTrayNotify;
    std::string strStatusBar;

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(nRelayUntil);
        READWRITE(nExpiration);
        READWRITE(nID);
        READWRITE(nTime);
        READWRITE(nLanguage);
        READWRITE(nPriority);

        READWRITE(strHeader);
        READWRITE(strMessage);
        READWRITE(strTrayNotify);
    )

    void SetNull();

    std::string ToString() const;
    void print() const;
};

class CNewsMessage : public CUnsignedNewsMessage
{
public:
    std::vector<unsigned char> vchMsg;
    std::vector<unsigned char> vchSig;

    CNewsMessage()
    {
        SetNull();
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(vchMsg);
        READWRITE(vchSig);
    )

    void SetNull();
    bool IsNull() const;
    uint256 GetHash() const;
    bool IsInEffect() const;    
    bool RelayTo(CNode* pnode) const;
    bool CheckSignature() const;
    bool ProcessMessage(bool fThread = true);

    static CNewsMessage getMessageByHash(const uint256 &hash);
};

#endif // CNEWSMESSAGE_H
