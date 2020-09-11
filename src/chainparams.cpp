// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 FXTC developers
// Copyright (c) 2019 Megacoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // Megacoin
    const char* pszTimestamp = "Boston Herald - 21/May/2013 - IRS Official to Take Fifth to Avoid Testifying"; //03-2018
    const CScript genesisOutputScript = CScript() << ParseHex("0411582e4e718df82c9d75a4886ab7602f0a1b866d81a4e44acba04e847ccd0514b97bee4a61fa73b1474d12851422b01565f244f722f318f1608258b74a3f3fe6") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 420000; // Megacoin
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 50; // - not used
        consensus.nMasternodePaymentsIncreaseBlock = 50; // - not used
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year  - not used
        consensus.nMasternodeCollateralMinimum = 4200; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100000; // MN collateral at infinity - not used

        consensus.nInstantSendKeepLock = 24;
// We need a seperate Update for that!! Limxdev 2019
        consensus.nBudgetPaymentsStartBlock = 1260000; // Fork 0.17
        consensus.nBudgetPaymentsCycleBlocks = 8064; // two weeks 
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day
// We need a seperate Update for that!! Limxdev 2019
        consensus.nSuperblockStartBlock = 1260000; // Fork 0.17
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x11618621285fb4f1a560b462a2f8c8eb44250aac1406d9fe2c90ffbc472e3ebb"); // Megacoin 500000
        consensus.BIP34Height = 500000; // Megacoin
        consensus.BIP34Hash = uint256S("0x11618621285fb4f1a560b462a2f8c8eb44250aac1406d9fe2c90ffbc472e3ebb"); // Megacoin 500000
        consensus.BIP65Height = 1050065; // Megacoin
        consensus.BIP66Height = 1070066; // Megacoin
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Megacoin: ToDo
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Megacoin
        consensus.nPowTargetTimespanV2 = 160 * 60; // Megacoin
        consensus.nPowTargetSpacing = 2.5 * 60; // Megacoin
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // Megacoin
        consensus.nMinerConfirmationWindow = 8064; // Megacoin
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nHeight = 1280000;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nHeight = 1280000;
        
        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nHeight = 1280000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000005dfab1790ba9fdd"); // Megacoin

        //0x0000000000000000000000000000000000000000000000000b6cc0f906e29fdd
        //823245007696863197
        //0x00000000000000000000000000000000000000000000000005dfab1790ba9fdd
        //423245007696863197

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xc69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf"); // Megacoin

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        // Megacoin
        pchMessageStart[0] = 0xed;
        pchMessageStart[1] = 0xe0;
        pchMessageStart[2] = 0xe4;
        pchMessageStart[3] = 0xee;
        nDefaultPort = 7951;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1369197853, 2084576387, 0x1e0ffff0, 1, 500 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        // Megacoin
        vSeeds.emplace_back("37.120.190.76");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("185.194.140.60");
        vSeeds.emplace_back("188.71.223.206");
        vSeeds.emplace_back("185.194.142.122");
        //vSeeds.emplace_back("185.194.142.122", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "mec"; // Megacoin

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "0490511F62A54A924FF13A2E43383325A65FD9CAA16C66C2334FA39E000D1229E74A47B22230E37CFBB42806902B0EB08249EBA6FD7913FEA0557F4697109D42E3";

        founderAddress = "M7uyx2is8rgnTsfxC28mTbuxmXqQPSTKAH";

        // Megacoin
        checkpointData = {
            {
                { 5215,    uint256S("0xf8a7fec79eeee3228499601c614b179d09b1b08bd58515be8a2795f6baafb493")},
                { 490000,  uint256S("0x957ffdc0ead5ac30a6bb914fbf68f285a32dcf091b47b621827e3b58e8e9bf66")},
                { 974448,  uint256S("0xc69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf")},
                { 1001795, uint256S("0x0555327b8f542a4b1fb3de8c4d988ea46e687b9bd8ce406e1b2aea49fe959391")},
                { 1253244, uint256S("0x058403d520ee00f1f2651eddb1e9a105145481cdfdc9a1b4eb0f8bd8ba68fa9f")},
                { 1269715, uint256S("0xab6b610ec05a663343e635a54213f4f4eac13c4d0703a104ae1ea9b72cf2a9b8")},
                { 1299115, uint256S("0x93ebdcb6c6378bf9c19f47aa3a1ffb1d18d8325636507c159f2ac90afb247ecc")},
                { 1328857, uint256S("0xfaaf11ce81c108079f675559ab1b9ca68352aec9f5b5efc00c3150332650fc82")},
            }
        };

        // MEC for faster loading
        consensus.nlastValidPowHashHeight = 1299115;
        chainTxData = ChainTxData{
            // Megacoin: data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1595274616, // * UNIX timestamp of last known number of transactions
            1000000,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1           // * estimated number of transactions per second after that timestamp
        };

        // FXTC TODO: we need to resolve fee calculation bug and disable fallback
        ///* disable fallback fee on mainnet */
        //m_fallback_fee_enabled = false;
        m_fallback_fee_enabled = true;
        //
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 420000; // Megacoin
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 10; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 10;
        consensus.nMasternodePaymentsIncreasePeriod = 25;
        consensus.nMasternodeCollateralMinimum = 10; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 1000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 4200;
        consensus.nSuperblockCycle = 24;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614");
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614"); // Megacoin
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Megacoin
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // Megacoin
        consensus.nPowTargetTimespanV2 = 160 * 60; // Megacoin
        consensus.nPowTargetSpacing = 2.5 * 60; // Megacoin
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // Megacoin
        consensus.nMinerConfirmationWindow = 2; // Megacoin
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1525132800; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1525132800; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)

        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1525132800; // April 23, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1746057600;   // 05/01/2025 @ 12:00am (UTC)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // Megacoin

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb"); // Megacoin

        // Megacoin
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1369197853, 2084576387, 0x1e0ffff0, 1, 500 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        /*
        // Megacoin
        vFixedSeeds.clear();
        vSeeds.clear();
        */
        vSeeds.emplace_back("188.68.52.172");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("188.71.223.206");
        vSeeds.emplace_back("185.194.142.122");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "testnet1mec";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04C637C5DBCA831BF02266B23E0F271B2689A5754A83BBE00B901C821D567AD23343007E21DA062F2D1C9231B23FDE29D34C7FC8F58292735CBB348F48F768C6F3";

        founderAddress = "mv5hH1xC3KQJr4ATDr1yQLmFhRYgVmjua6";

        // Megacoin
        checkpointData = {
            {
                {2, uint256S("0x222db35707d9af172ea5c23582338874d1f2956b1850f857c50a3e6f2c6f313")},
            }
        };

        consensus.nlastValidPowHashHeight = 0;
        
        chainTxData = ChainTxData{
            // Megacoin: data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1490495580, // * UNIX timestamp of last known number of transactions
            449775,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0       // * estimated number of transactions per second after that timestamp
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMinimumSubsidy = 10000.00000000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nMasternodeCollateralMinimum = 1; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x89");
        consensus.BIP34Height = 5000; // Megacoin: BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256(); // Megacoin
        consensus.BIP65Height = 5000; // Megacoin: BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 5000; // Megacoin: BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // Megacoin
        consensus.nPowTargetSpacing = 10 * 60; // Megacoin
        consensus.nPowTargetTimespanV2 = 160 * 60; // Megacoin
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //
        //

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // Megacoin
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1492973331, 9377, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        founderAddress = "cXdevX1jzA1Bd6hZVTcTEPh5NH8iDx1s4Z";

        // Megacoin
        checkpointData = {
            {
                {999999, uint256S("0x0000000069e244f73d78e8fd29ba2fd2ed618bd6fa2ee92559f542fdb26e7c1d")},
            }
        };

        consensus.nlastValidPowHashHeight = 0;
        
        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "regtest1mec";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
