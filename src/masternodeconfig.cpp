// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 FXTC developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <masternodeconfig.h>
//#include <netbase.h>
#include <util.h>
#include <utilstrencodings.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

CMasternodeConfig masternodeConfig;

void CMasternodeConfig::add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex) {
    CMasternodeEntry cme(alias, ip, privKey, txHash, outputIndex);
    entries.push_back(cme);
}

bool CMasternodeConfig::read(std::string& strErr) {
    int linenumber = 1;
    boost::filesystem::path pathMasternodeConfigFile = GetConfigFile(gArgs.GetArg(MASTERNODE_CONF_FILENAME_ARG, MASTERNODE_CONF_FILENAME));
    boost::filesystem::ifstream streamConfig(pathMasternodeConfigFile);
    boost::filesystem::path pathConfigFile = GetConfigFile(gArgs.GetArg(MASTERNODE_CONF_FILENAME_ARG, BITCOIN_CONF_FILENAME));
    boost::filesystem::ifstream streamConfig2(pathConfigFile);
    //MEC Begin
    if (!streamConfig2.good()) {
        FILE* configFile = fopen(pathConfigFile.string().c_str(), "a");
        if (configFile != NULL) {
                        std::string strHeader = "# Megacoin config file v 1.1\n"
                        "# Network Nodes\n"
                        "addnode=37.120.186.85\n"
                        "addnode=37.120.190.76\n"
                        "addnode=185.194.140.60\n"
                        "addnode=185.194.142.125\n"
                        "addnode=115.159.124.86\n"
                        "addnode=120.243.237.236\n"
                        "addnode=120.79.91.232\n"
                        "addnode=125.94.91.54\n"
                        "addnode=178.200.230.252\n"
                        "addnode=185.194.142.125\n"
                        "addnode=188.42.33.124\n"
                        "# Masternode preparation (optional) - Note - You have to remove #\n"
                        "#masternodeaddr=1.2.3.4:7951\n"
                        "#externalip=1.2.3.4:7951\n"
                        "#masternode=1\n"
                        "#masternodeprivkey=yourkey\n"
                        "# Server preparation (optional) - Note - You have to remove #\n"
                        "#rpcuser=User\n"
                        "#rpcpassword=mystrongpassword\n"
                        "#rpcport=7952\n"
                        "# Miscellaneous #\n"
                        "#nodebuglogfile=1\n"
                        "daemon=1\n"
                        "listen=1\n"
                        "logtimestamps=1\n"
                        "maxconnections=64\n";
            fwrite(strHeader.c_str(), std::strlen(strHeader.c_str()), 1, configFile);
            fclose(configFile);
            //MEC End
        }
                return true; // Nothing to read, so just return
    }
    //We add a megacoin.conf by default

    if (!streamConfig.good()) {
        FILE* configFile = fopen(pathMasternodeConfigFile.string().c_str(), "a");
        if (configFile != NULL) {
            std::string strHeader2 = "# Masternode config file\n"
                          "# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index\n"
                          "# Example: mn1 127.0.0.2:9468 7y9mBodVbq5nytRyZNg169ABTeKffDNqekCPiXKWGP2ZYDRHYbk ddaa0ebeed10aef980adbf9579718aed85b533eb5816adde661221f656382dd6 0\n";
            fwrite(strHeader2.c_str(), std::strlen(strHeader2.c_str()), 1, configFile);
            fclose(configFile);
        }
        return true; // Nothing to read, so just return
    }

    for(std::string line; std::getline(streamConfig, line); linenumber++)
    {
        if(line.empty()) continue;

        std::istringstream iss(line);
        std::string comment, alias, ip, privKey, txHash, outputIndex;

        if (iss >> comment) {
            if(comment.at(0) == '#') continue;
            iss.str(line);
            iss.clear();
        }

        if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
            iss.str(line);
            iss.clear();
            if (!(iss >> alias >> ip >> privKey >> txHash >> outputIndex)) {
                strErr = _("Could not parse masternode.conf") + "\n" +
                        strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"";
                streamConfig.close();
                return false;
            }
        }

        int port = 0;
        std::string hostname = "";
        SplitHostPort(ip, port, hostname);
        if(port == 0 || hostname == "") {
            strErr = _("Failed to parse host:port string") + "\n"+
                    strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"";
            streamConfig.close();
            return false;
        }
        int nDefaultPort = Params().GetDefaultPort();
        if(Params().NetworkIDString() == CBaseChainParams::MAIN) {
            if(port != nDefaultPort) {
                strErr = _("Invalid port detected in masternode.conf") + "\n" +
                        strprintf(_("Port: %d"), port) + "\n" +
                        strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"" + "\n" +
                        strprintf(_("(must be %d for mainnet)"), nDefaultPort);
                streamConfig.close();
                return false;
            }
        } else if(port == nDefaultPort) {
            strErr = _("Invalid port detected in masternode.conf") + "\n" +
                    strprintf(_("Line: %d"), linenumber) + "\n\"" + line + "\"" + "\n" +
                    strprintf(_("(%d could be used only on mainnet)"), nDefaultPort);
            streamConfig.close();
            return false;
        }


        add(alias, ip, privKey, txHash, outputIndex);
    }

    streamConfig.close();
    return true;
}
