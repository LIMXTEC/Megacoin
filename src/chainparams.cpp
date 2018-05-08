// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

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
    //const char* pszTimestamp = "Der Tagesspiegel 06/Jan/2014 Henry Maske, famous fighter, is now 50 years old";
    //const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;

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
        consensus.nSubsidyHalvingInterval = 420000;
        consensus.BIP34Height = 500000; // MegaCoin BIP34 height 
        consensus.BIP34Hash = uint256S("11618621285fb4f1a560b462a2f8c8eb44250aac1406d9fe2c90ffbc472e3ebb"); // MegaCoin BIP34 Hash
        consensus.BIP65Height = 1050065; // c69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf
        consensus.BIP66Height = 1070066; // c69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf
        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // To Do
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60;
		consensus.nPowTargetTimespanV2 = 160 * 60;
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // Segwit direct online
		// Megacoin with 75%
        //Limx DevMinimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period, (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments. 
        consensus.nMinerConfirmationWindow = 8064; //2016 nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1525132800; // 05/01/2018 @ 12:00am (UTC)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000"); // to do

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xc69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf"); //974448

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         *
	 */pchMessageStart[0] = 0xed;
        pchMessageStart[1] = 0xe0;
        pchMessageStart[2] = 0xe4;
        pchMessageStart[3] = 0xee;
        nDefaultPort = 7951;
        nPruneAfterHeight = 100000;
		//Time Nonce ?
		// genesis = CreateGenesisBlock(1492973331, 9377, 0x1e0ffff0, 1, 50 * COIN); // MegaCoin 1492973331, 3716037
        genesis = CreateGenesisBlock(1369197853, 2084576387, 0x1e0ffff0, 1, 500 * COIN); // MegaCoin 1492973331, 3716037
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        // Note that of those with the service bits flag, most only support a subset of possible options
		
        vSeeds.emplace_back("37.120.190.76", false);
        vSeeds.emplace_back("37.120.186.85", false);
		vSeeds.emplace_back("185.194.140.60", false);
        vSeeds.emplace_back("188.71.223.206", false);
        vSeeds.emplace_back("185.194.142.122", false);
		
		/*
		vSeeds.emplace_back("seed-a.litecoin.loshan.co.uk", true);
        vSeeds.emplace_back("dnsseed.thrasher.io", true);
        vSeeds.emplace_back("dnsseed.litecointools.com", true);
        vSeeds.emplace_back("dnsseed.litecoinpool.org", true);
        vSeeds.emplace_back("dnsseed.koin-project.com", false);
        */
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,178);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData){
        {
			{ 5215, uint256S("0xf8a7fec79eeee3228499601c614b179d09b1b08bd58515be8a2795f6baafb493")},
			{ 490000, uint256S("0x957ffdc0ead5ac30a6bb914fbf68f285a32dcf091b47b621827e3b58e8e9bf66")},
			{ 974448, uint256S("0xc69f5106c4f3ba92cfdcd13d20135694de444087e975ba7ea89e3532708bfccf")},
			{ 1001795, uint256S("0x0555327b8f542a4b1fb3de8c4d988ea46e687b9bd8ce406e1b2aea49fe959391")},
        }
		};

        chainTxData = ChainTxData{
            // Data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1521658394, // * UNIX timestamp of last known number of transactions
            1000000,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            1         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 420000; // MegaCoin number of blocks until halving on 1st Nov. 2016 09:17:52 PM
        consensus.BIP34Height = 0; // MegaCoin BIP34 height
        consensus.BIP34Hash = uint256S("5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614"); // MegaCoin BIP34 Hash
        consensus.BIP65Height = 0; // c49bfa33dd4d76a6a05f93c1eb4310993ce00c7a8a9ee23c76164ddc2eecbdef
        consensus.BIP66Height = 0; // 83703951207fcf9e22516f97603f8621299de483e85d85e8a86f7b0fd32717dc
        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // MegaCoin PoW Limit
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // Megacoin Testnet 1% of 2
        consensus.nMinerConfirmationWindow = 2; // MegaCoin nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1525132800; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1525132800; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1525132800; // April 23, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1746057600; // 05/01/2025 @ 12:00am (UTC)
        // The best chain should have at least this much work.
		
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb"); //2

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        nDefaultPort = 19444;;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1369197853, 2084576387, 0x1e0ffff0, 1, 500 * COIN); // MegaCoin 1492973331, 3716037
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

          vFixedSeeds.clear();
        // Note that of those with the service bits flag, most only support a subset of possible options
		vSeeds.emplace_back("188.68.52.172", true);
        vSeeds.emplace_back("37.120.186.85", true);
        vSeeds.emplace_back("37.120.186.85", true);
        vSeeds.emplace_back("188.71.223.206", true);
        vSeeds.emplace_back("185.194.142.122", false);

		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
		
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData){
        {
			{ 2, uint256S("0x222db35707d9af172ea5c23582338874d1f2956b1850f857c50a3e6f2c6f313")},
		}
        };

        chainTxData = ChainTxData{
            // Data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1490495580, // * UNIX timestamp of last known number of transactions
            449775,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0         // * estimated number of transactions per second after that timestamp
        };

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
        consensus.BIP34Height = 5000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 5000; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 5000; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xf0;
        pchMessageStart[2] = 0xf4;
        pchMessageStart[3] = 0xfe;
        nDefaultPort = 19444;
nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1492973331, 9377, 0x1e0ffff0, 1, 0 * COIN); // MegaCoin 1492973331, 3716037
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7520788e2d99eec7cf6cf7315577e1268e177fff94cb0a7caf6a458ceeea9ac2"));
        assert(genesis.hashMerkleRoot == uint256S("0x6065d08d755e00a90449abe8a0923d0622feb6f7ab3f435c337369334119e636"));

        // Note that of those with the service bits flag, most only support a subset of possible options
		vSeeds.emplace_back("188.68.52.172", true);
        vSeeds.emplace_back("37.120.186.85", true);
        vSeeds.emplace_back("37.120.186.85", true);
        vSeeds.emplace_back("188.71.223.206", true);
        vSeeds.emplace_back("185.194.142.122", false);

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData) {
            {
                { 999999, uint256S("0x0000000069e244f73d78e8fd29ba2fd2ed618bd6fa2ee92559f542fdb26e7c1d")},
            }
        };
 
        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
  
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
