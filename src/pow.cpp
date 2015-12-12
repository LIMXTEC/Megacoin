// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include "bignum.h" // LimxDev for KGW

static CBigNum bnProofOfWorkLimit(~uint256(0) >> 20);

unsigned int GetNextWorkRequired_BTC(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % Params().Interval() != 0)
    {
        if (Params().AllowMinDifficultyBlocks())
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + Params().TargetSpacing()*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % Params().Interval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Megacoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = Params().Interval()-1;
    if ((pindexLast->nHeight+1) != Params().Interval())
        blockstogoback = Params().Interval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);
    if (nActualTimespan < Params().TargetTimespan()/4)
        nActualTimespan = Params().TargetTimespan()/4;
    if (nActualTimespan > Params().TargetTimespan()*4)
        nActualTimespan = Params().TargetTimespan()*4;

    // Retarget
    uint256 bnNew;
    uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Megacoin: intermediate uint256 can overflow by 1 bit
    bool fShift = bnNew.bits() > 235;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= Params().TargetTimespan();
    if (fShift)
        bnNew <<= 1;

    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();

    /// debug print
    LogPrintf("GetNextWorkRequired RETARGET\n");
    LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

unsigned int static KimotoGravityWell2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, uint64_t BlocksTargetSpacing, uint64_t PastBlocksMin, uint64_t PastBlocksMax) {
    /* current difficulty formula, megacoin - kimoto gravity well */
    const CBlockIndex  *BlockLastSolved                                = pindexLast;
    const CBlockIndex  *BlockReading                                = pindexLast;
    const CBlockHeader *BlockCreating                                = pblock;
    BlockCreating                                = BlockCreating;
    uint64_t                                PastBlocksMass                                = 0;
    int64_t                                PastRateActualSeconds                = 0;
    int64_t                                PastRateTargetSeconds                = 0;
    double                                PastRateAdjustmentRatio                = double(1);
    CBigNum                                PastDifficultyAverage;
    CBigNum                                PastDifficultyAveragePrev;
    double                                EventHorizonDeviation;
    double                                EventHorizonDeviationFast;
    double                                EventHorizonDeviationSlow;
    int KGW3_var = 550000;
  //  int64_t LastBlockTime = 0;
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) { return bnProofOfWorkLimit.GetCompact(); }

    int64_t LatestBlockTime = BlockLastSolved->GetBlockTime();

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;

        if (i == 1)        { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
        else                { PastDifficultyAverage = ((CBigNum().SetCompact(BlockReading->nBits) - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev; }
        PastDifficultyAveragePrev = PastDifficultyAverage;

        if (BlockReading->nHeight > KGW3_var && LatestBlockTime < BlockReading->GetBlockTime()) {
            //eliminates the ability to go back in time
            LatestBlockTime = BlockReading->GetBlockTime();
        }

        PastRateActualSeconds                        = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds                        = TargetBlocksSpacingSeconds * PastBlocksMass;
        PastRateAdjustmentRatio                        = double(1);

        if (BlockReading->nHeight > KGW3_var){
            //this should slow down the upward difficulty change
            if (PastRateActualSeconds < 20) { PastRateActualSeconds = 20; }
        }
        else {
            if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        }


        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio                        = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        if (BlockReading->nHeight > KGW3_var) {
                        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(72)), -1.228));
                } else {
                        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(144)), -1.228));
                }
        EventHorizonDeviationFast                = EventHorizonDeviation;
        EventHorizonDeviationSlow                = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
            if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast)) { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    CBigNum bnNew(PastDifficultyAverage);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
        bnNew *= PastRateActualSeconds;
        bnNew /= PastRateTargetSeconds;
    }
    
    // KGW3 Securty Option 1 Limx Dev 26-11-2015 Not used !
   /*
       if(LastBlockTime > 0){
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
            nActualTimespan += Diff;
        }
        LastBlockTime = BlockReading->GetBlockTime();

    if (nActualTimespan < PastRateTargetSeconds/3)
        nActualTimespan = PastRateTargetSeconds/3;
    if (nActualTimespan > PastRateTargetSeconds*3)
        nActualTimespan = PastRateTargetSeconds*3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= PastRateTargetSeconds;
    */
    
    
          
    // Limxdev cgminer-0 KGW3 VAR Diff Break version 2 Limx Dev 7-12-2015
	// LogPrintf("Prediff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
	// Reduce difficulty if current block generation time has already exceeded maximum time limit.
	const int nLongTimeLimit   = 1 * 60 * 60; 
	LogPrintf("prediff %d \n", nLongTimeLimit);LogPrintf(" %d Block", BlockReading->nHeight );
	LogPrintf("Time since last Block -  %d \n", (pblock-> nTime - pindexLast->GetBlockTime());
	if (BlockReading->nHeight > KGW3_var){ 
	if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit )  // 1 hours
	{	
	const int nLongTimebnNew   = 2; bnNew = bnNew * nLongTimebnNew;
       	LogPrintf("<MEC> KGW3 1h cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}
		if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit *2)  // 2 hours 
	{	
	const int nLongTimebnNew   = 3; bnNew = bnNew * nLongTimebnNew;
       	LogPrintf("<MEC> KGW3 1h cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}
		if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit *3)  // 3 hours 
	{	
	const int nLongTimebnNew   = 4; bnNew = bnNew * nLongTimebnNew;
       LogPrintf("<MEC> KGW3 1h - cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}
		if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit *4)  // 4 hours
	{	
	const int nLongTimebnNew   = 5; bnNew = bnNew * nLongTimebnNew;
       	LogPrintf("<MEC> KGW3 3h - cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}
	if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit *5)  // Backupfunction after 5 hours 
	{	
	const int nLongTimebnNew   = 100; bnNew = bnNew * nLongTimebnNew;
       	LogPrintf("<MEC> KGW3 5h -  cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
	}
	}

  

    if (bnNew > bnProofOfWorkLimit) { bnNew = bnProofOfWorkLimit; }

    /// debug print
    /*
    LogPrintf("Difficulty Retarget - Kimoto Gravity Well\n");
    LogPrintf("PastRateAdjustmentRatio = %g\n", PastRateAdjustmentRatio);
    LogPrintf("Before: %08x  %s\n", BlockLastSolved->nBits, CBigNum().SetCompact(BlockLastSolved->nBits).getuint256().ToString().c_str());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.getuint256().ToString().c_str());
  */
    return bnNew.GetCompact();
} //Limxdev

unsigned int static GetNextWorkRequired_KGW2(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    static const int64_t        BlocksTargetSpacing                        = 2.5 * 60; // 2.5 minutes
    unsigned int                TimeDaySeconds                                = 60 * 60 * 24;
    int64_t                       PastSecondsMin                                = TimeDaySeconds * 0.25;
    int64_t                       PastSecondsMax                                = TimeDaySeconds * 7;
    uint64_t                      PastBlocksMin                                = PastSecondsMin / BlocksTargetSpacing;
    uint64_t                      PastBlocksMax                                = PastSecondsMax / BlocksTargetSpacing;

    return KimotoGravityWell2(pindexLast, pblock, BlocksTargetSpacing, PastBlocksMin, PastBlocksMax);
} //Limxdev

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    int DiffMode = 0;
    int BlockHeight = 0;

    BlockHeight = pindexLast->nHeight+1;

    //comment this out in a second
    LogPrintf("BlockHeight = %d\n", BlockHeight);

    if(BlockHeight > 50000){
        DiffMode = 2;
    }

    switch(DiffMode){
        case 2:
            return GetNextWorkRequired_KGW2(pindexLast, pblock);
            break;
        default:
            return GetNextWorkRequired_BTC(pindexLast, pblock);

    }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (Params().SkipProofOfWorkCheck())
       return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}
