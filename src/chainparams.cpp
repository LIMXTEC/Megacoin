// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin developers
// Copyright (c) 2014-2017 The Megacoin developers
// Copyright (c) 2013-2079 Dr. Kimoto Chan
// Copyright (c) 2013-2079 The Megacoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
 
// LimxDev 07.08.2015
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		(   5215, uint256("0xf8a7fec79eeee3228499601c614b179d09b1b08bd58515be8a2795f6baafb493"))
		(   7333, uint256("0xf798f245214a5ed83c69d362a88c48047c82675d3bf84b0c88d6f1c71ed4b372"))
		(   9850, uint256("0x03359392dab8f47cd70626bb978c7658a385a49570bd953a1c9f8ceed4ff8be6"))
		(  10000, uint256("0xe17057f2114a45827acbcfe2a55b273ccabd3dc8982765f7a3fbca166811909b"))
		(  15480, uint256("0xf682a44431fff21c5d412f97e92f3340c59527626f6b5a4c6b85de78e009f177"))
		(  17625, uint256("0x5c4df33d72bef0c9e95bcce997e0c1d5985e957a16aebbf463a8bbee1ab9eb4b"))
		(  21190, uint256("0xe960216ff0e3ebae632af05792f0a11bf8a8f61e4f5ef43fcfd84b6e8053ec59"))
		(  35500, uint256("0xb88a54ce5d247a46166ff28228c62efc495cfb8d0dadfa895bced191cb261c90"))
		(  39500, uint256("0x3eb61d9897009caa6ed492fc154121849166d7a627f26322eae9cf32c7dc753b"))
		(  44400, uint256("0xeb5af032f88981810f31f13e38e33c86585dbf963ea6de199382706dc5b3aee4"))
		(  59300, uint256("0x558cf1f1fe31eb0816f9fc73900133564c29b50b626cbf82c69860fd3583653c"))
		(  62767, uint256("0x5bd5d25c8a19b764634435b9ab1121b4678fbf6e6ad771b252f75f3cdfa82131"))		
		(  96800, uint256("0xf972b9421ac790af82cd63f5db1dbbee114ce3476486d4335f46c6d7d8897671"))
		( 106980, uint256("0x0144e525eb188106e435af296caf2bf707e174c65b1b50361b9dfeb942123cab"))
		( 184000, uint256("0x61974eed4dd83c7b4b4954593769fe1939e00d701bd5cfd17d30eeb986dae67c"))		
		( 201050, uint256("0xf6f0890c7d94ac3596fbb19050a95edaea14fca8706b2b55408fe202f413153b"))
		( 221000, uint256("0x88d40255918470d178b1f171f58543b74b130982a29eda026416ca643592132d"))
		( 241000, uint256("0x53c964a043a98d8731a4ad62e8d155a619d4f2d413d4d6352e5cba5eb6c6b786"))
		( 261300, uint256("0xcd9d91664a49bbf8c1c504420bf6ea5e2ac3a68e55344396d4bbf59162ff4261"))
		( 284000, uint256("0x5cdce62308c7e77c6aa93ab0e754116fbb14d4a0972da5b6465a31b047e47cb2"))
		( 304000, uint256("0x6ab3a605154bd90aa748ea07de6d3f98d42f32b8a56ba1d993ea0837fa28087c"))
		( 334100, uint256("0xb44d860b7332c4d235b013270dedaec9b0e8e4bfe701cc1ef322b807c41c6b7d"))
		( 392675, uint256("0x26693075b87f5b6394381fe8590cd69a93e965a66d329786a1abfe0bc2fcdc5c"))		
		( 401000, uint256("0xf4b402f749719cb89251303fb376eb9d51cc05618e16585b9fdb13d02387e620"))
		( 425000, uint256("0xab5473a5b41bb86d4e5b45ff079b239ea8cd4be83da295ab5b3f35a386a5892c"))
		( 460100, uint256("0x122716bde5b0cc2d7ec4d95784e6f483926c2227b8af927753d24326390c654f"))
		( 490000, uint256("0x957ffdc0ead5ac30a6bb914fbf68f285a32dcf091b47b621827e3b58e8e9bf66"))		
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1384183736, // * UNIX timestamp of last checkpoint block
        365070,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        60000.0     // * estimated number of transactions per day after checkpoint
		// Limx Dev 07.08.2015
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (   2236, uint256("0xfae5f0ff729c2a296d1aa486818a35279babc40da15dba5ceef047be2c4a7a7a")) // Limx Dev 07.08.2015
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1370212592, // Limx Dev 07.08.2015
        2235,
        300
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
		pchMessageStart[0] = 0xed;
        pchMessageStart[1] = 0xe0;
        pchMessageStart[2] = 0xe4;
        pchMessageStart[3] = 0xee; // Limx Dev 07.08.2015 _ LIMX SUPPORT
        vAlertPubKey = ParseHex("04680D5270D3E6BA9E2671D889778FC4A0753FF036171D553A179635DDE67146481AB4EA7A6856CBB6A97AE16E7820F2F318E88F21E9BCFB0380E52BD63306C05C");
        nDefaultPort = 7951;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nSubsidyHalvingInterval = 420000;  // Limx Dev 
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        nTargetSpacing = 2.5 * 60; // 2.5 minutes

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         * 
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "Boston Herald - 21/May/2013 - IRS Official to Take Fifth to Avoid Testifying"; //Limx Dev 07.08.2015
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 500 * COIN; // Limx Dev 07.08.2015
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0411582e4e718df82c9d75a4886ab7602f0a1b866d81a4e44acba04e847ccd0514b97bee4a61fa73b1474d12851422b01565f244f722f318f1608258b74a3f3fe6") << OP_CHECKSIG; // Limx Dev 07.08.2015
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1369197853; // Limx Dev 07.08.2015
        genesis.nBits    = 0x1e0ffff0; // Limx Dev 07.08.2015
        genesis.nNonce   = 2084576387; // Limx Dev 07.08.2015

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        vSeeds.push_back(CDNSSeedData("185.194.142.125", "185.194.142.125")); // Limx Dev 07.08.2015 Online
        vSeeds.push_back(CDNSSeedData("185.194.140.60", "185.194.140.60")); // Limx Dev 07.08.2015 Off
       // vSeeds.push_back(CDNSSeedData("megacoin.in", "dnsseed.megacoin.in")); // Limx Dev 07.08.2015 Off
        //vSeeds.push_back(CDNSSeedData("144.76.118.88", "144.76.118.88")); //Blockr Explorer Online
       // vSeeds.push_back(CDNSSeedData("108.61.179.50", "108.61.179.50")); // Limx Dev 07.08.2015 LIMX Support

        base58Prefixes[PUBKEY_ADDRESS] = list_of(50); // Limx Dev 07.08.2015 Megacoin addresses start with M
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(178); // Limx Dev 07.08.2015  Pubkey +128 ??
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E); // Limx Dev 07.08.2015 "xpub "
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4); // Limx Dev 07.08.2015 "xpriv"

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        // Megacoin: Mainnet v2 enforced as of block 710k
        nEnforceV2AfterHeight = 710000;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xfd; // Limx Dev 07.08.2015 (main.cpp 2844)
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        vAlertPubKey = ParseHex("04826AC11FCF383A1E0F21E2A76807D082FF4E7F139111A7768E4F5A35A5653A2D44A8E19BC8B55AEDC9F9238D424BDC5EBD6D2BAF9CB3D30CEDEA35C47C8350A0"); // Limx Dev 07.08.2015
        nDefaultPort = 17951;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        nTargetSpacing = 2.5 * 60; // 2.5 minutes

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1369198853; // Limx Dev 07.08.2015
        genesis.nNonce = 386245382; // Limx Dev 07.08.2015
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xf9e3f18eaa2404b23c57ed9961d7e777e95a1f2c88dfd373e5ddadc8ed343cc1")); // Limx Dev 07.08.2015 

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("108.61.179.50", "108.61.179.50"));
        //vSeeds.push_back(CDNSSeedData("xurious.com", "testnet-seed.ltc.xurious.com"));
        //vSeeds.push_back(CDNSSeedData("wemine-testnet.com", "dnsseed.wemine-testnet.com"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);  // Limx Dev 07.08.2015
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);  // Limx Dev 07.08.2015
        base58Prefixes[SECRET_KEY]     = list_of(239);  // Limx Dev 07.08.2015
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF); 
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        // Megacoin: Testnet v2 enforced as of block 400k
        nEnforceV2AfterHeight = 400000;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {  // Limx Dev 07.08.2015  !! BTC Original !! 
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xfd; // Limx Dev 07.08.2015 (main.cpp 2844)
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        nTargetSpacing = 2.5 * 60; // 2.5 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        genesis.nTime = 1369198853;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 386245382;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 19444;
        assert(hashGenesisBlock == uint256("0xf9e3f18eaa2404b23c57ed9961d7e777e95a1f2c88dfd373e5ddadc8ed343cc1"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        // Megacoin: v2 enforced using Bitcoin's supermajority rule
        nEnforceV2AfterHeight = -1;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;

        // Megacoin: v2 enforced using Bitcoin's supermajority rule
        nEnforceV2AfterHeight = -1;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
