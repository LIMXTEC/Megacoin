// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2013-2079 Dr. Kimoto Chan
// Copyright (c) 2013-2079 The Megacoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		//(	0, hashGenesisBlock)
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
    ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1384183736, // * UNIX timestamp of last checkpoint block
        365070,   	// * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        60000.0     // * estimated number of transactions per day after checkpoint
    };
	
    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
		//(	0, hashGenesisBlock)
        (   2236, uint256("0xfae5f0ff729c2a296d1aa486818a35279babc40da15dba5ceef047be2c4a7a7a"))
    ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1370212592,
        2235,
        300
    };
	
    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
