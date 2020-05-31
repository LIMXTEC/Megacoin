// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2018 FXTC developers
// Copyright (c) 2019 Megacoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>
// FXTC BEGIN
#include <spork.h>
// FXTC END

// Megacoin
//#include <bignum.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <consensus/consensus.h>


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    /*
    unsigned int nBits = DarkGravityWave(pindexLast, pblock, params);

    // Dead lock protection will halve work every block spacing when no block for 2 * number of active algos * block spacing (FxTC: every two minutes if no block for 10 minutes)
    int nHalvings = (pblock->GetBlockTime() - pindexLast->GetBlockTime()) / (params.nPowTargetSpacing * 2) - ALGO_ACTIVE_COUNT + 1;
    if (nHalvings > 0)
    {
        const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
        arith_uint256 bnBits;
        bnBits.SetCompact(nBits);

        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2x block spacing
        // then allow mining of a min-difficulty block.
        // Also can not be less than PoW limit.
        if (params.fPowAllowMinDifficultyBlocks || (bnPowLimit >> nHalvings) < bnBits)
            bnBits = bnPowLimit;
        else
            bnBits <<= nHalvings;

        nBits = bnBits.GetCompact();
    }

    return nBits;
    */

    // Megacoin
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();
    if (params.fPowNoRetargeting && params.fPowAllowMinDifficultyBlocks )
    {
        return nProofOfWorkLimit; // Remove old KGW
    }

    int fork1 = 1000000;
    int fork2 = 21000;

    // Megacoin Miningalgo switch
	// 1571832146 Wednesday, 23. October 2019 12:02:26
	// please check also block.cpp:L62
    // We have a timerange from 24 hours  to find a new block
    if(pblock->GetBlockTime() >= HASH_FORK_TIME_1 && pindexLast->GetBlockTime() <= HASH_FORK_TIME_1 + 86400) 
        {
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*24) 
            {
                //consensus.nPowTargetSpacing = 2.5 * 60; // Megacoin	
                //This should be one hour then is this function possible
                LogPrintf("Megacoin Hashalgoupdate Test!!! \n");
                return nProofOfWorkLimit;
            }
        }
        // We have a timerange from 24 hours  to find a new block
    if(pblock->GetBlockTime() >= HASH_FORK_TIME_2 && pindexLast->GetBlockTime() <= HASH_FORK_TIME_2 + 86400)
        {
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*24) 
            {
                //consensus.nPowTargetSpacing = 2.5 * 60; // Megacoin	
                //This should be one hour then is this function possible
                LogPrintf("Megacoin Hashalgoupdate MEGA MEC!!! \n");
                return nProofOfWorkLimit;
            }
        }

    if (pindexLast->nHeight+1 <= fork1) {
        return nProofOfWorkLimit;
    }

    if (pindexLast->nHeight+1 <= fork2)
    {
    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // MegaCoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
    }

    else
    {
        // Genesis block
        if (pindexLast == NULL)
            return nProofOfWorkLimit;

        // Only change once per difficulty adjustment interval
        if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2() != 0)
        {
            if (params.fPowAllowMinDifficultyBlocks)
            {
                // Special difficulty rule for testnet:
                // If the new block's timestamp is more than 2* 10 minutes
                // then allow mining of a min-difficulty block.
                if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                    return nProofOfWorkLimit;
                else
                {
                    // Return the last non-special-min-difficulty-rules-block
                    const CBlockIndex* pindex = pindexLast;
                    while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentIntervalV2() != 0 && pindex->nBits == nProofOfWorkLimit)
                        pindex = pindex->pprev;
                    return pindex->nBits;
                }
            }
            // LogPrintf("difficulty adjustment interval %d  \n",(pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2());
            return pindexLast->nBits;
        }

        // Go back by what we want to be 14 days worth of blocks
        // Litecoin: This fixes an issue where a 51% attack can change difficulty at will.
        // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
        int blockstogoback2 = params.DifficultyAdjustmentIntervalV2()-1;
        if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentIntervalV2())
            blockstogoback2 = params.DifficultyAdjustmentIntervalV2();

        // Go back by what we want to be 14 days worth of blocks
        const CBlockIndex* pindexFirst = pindexLast;
        for (int i = 0; pindexFirst && i < blockstogoback2; i++)
            pindexFirst = pindexFirst->pprev;

        assert(pindexFirst);
        return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
    }
}


unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    /*
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
    */

    // Megacoin
    int fork2 = 21000;
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Initial //64_15 Written by Limx Dev 04/2017
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    bool fShift;
    // Initial
    if (pindexLast->nHeight+1 <= fork2)
    {
        if (nActualTimespan < params.nPowTargetTimespan/4)
            nActualTimespan = params.nPowTargetTimespan/4;
        if (nActualTimespan > params.nPowTargetTimespan*4)
            nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // MegaCoin: intermediate uint256 can overflow by 1 bit
    fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();

    }
    else
    {
        if (nActualTimespan < params.nPowTargetTimespanV2/1.15)
            nActualTimespan = params.nPowTargetTimespanV2/1.15;
        if (nActualTimespan > params.nPowTargetTimespanV2*1.15)
            nActualTimespan = params.nPowTargetTimespanV2*1.15;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // MegaCoin: intermediate uint256 can overflow by 1 bit
    fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespanV2;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
    }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

/*
// FXTC BEGIN
unsigned int GetHandbrakeForce(int32_t nVersion, int nHeight)
{
    int32_t nVersionAlgo = nVersion & ALGO_VERSION_MASK;

    // NIST5 braked and disabled
    if (nVersionAlgo == ALGO_NIST5)
    {
        if (nHeight >= 21000) return 4070908800;
        if (nHeight >= 19335) return 20;
    }

    if (nHeight >= sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_HEIGHT))
    {
        switch (nVersionAlgo)
        {
            case ALGO_SHA256D: return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_SHA256D);
            case ALGO_SCRYPT:  return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_SCRYPT);
            case ALGO_NIST5:   return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_NIST5);
            case ALGO_LYRA2Z:  return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_LYRA2Z);
            case ALGO_X11:     return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_X11);
            case ALGO_X16R:    return sporkManager.GetSporkValue(SPORK_MEGACOIN_01_HANDBRAKE_FORCE_X16R);
            default:           return 1; // FXTC TODO: we should not be here
        }
    }

    return 1; // FXTC TODO: we should not be here
}
// FXTC END
*/
