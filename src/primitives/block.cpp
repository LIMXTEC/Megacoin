// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers\n// Copyright (c) 2009-2016 The Litecoin Core developers\n// Copyright (c) 2009-2016 The Megacoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"
#include "crypto/scrypt.h" // MegaCoin Scrypt
#include "crypto/hashblock.h" // MegaCoin TimeTravel

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
	/*
		if(GetBlockTime() >= 1493124696) //Human time (GMT): Tue, 25 Apr 2017 12:51:36 GMT
		{
				return HashTimeTravel(BEGIN(nVersion), END(nNonce), GetBlockTime()); // MegaCoin TimeTravel
		
		}
		else
		{
			*/
				uint256 thash;
				scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash)); // MegaCoin Scrypt
				return thash;
		//}
		
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
