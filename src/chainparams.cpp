// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.nTime = nTimeTx;

    CBlock genesis;
    genesis.nTime    = nTimeBlock;
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
static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Look at how a single candle can both defy and define the darkness";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
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

#define NEVER 2147485546

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 8388608;
        consensus.BIP34Hash = uint256S("0000000000000000000000000000000000000000000000000000000000000000");
        consensus.powLimit =            uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.bnInitialHashTarget = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nTargetTimespan = 24 * 60 * 60;                                   // https://github.com/TurboStake/TurboStake/blob/master/src/main.cpp L979
        consensus.nStakeTargetSpacing = 2.5 * 60;                                   // https://github.com/TurboStake/TurboStake/blob/master/src/main.h L44
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing;       // https://github.com/TurboStake/TurboStake/blob/master/src/main.cpp L980
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 2;                                  // https://github.com/TurboStake/TurboStake/blob/master/src/main.h L45
        consensus.nStakeMaxAge = 60 * 60 * 24 * 365 * 65;                           // https://github.com/TurboStake/TurboStake/blob/master/src/main.h L46
        consensus.nModifierInterval = 3 * 60 * 60;                                  // https://github.com/TurboStake/TurboStake/blob/master/src/kernel.h L10
        consensus.nCoinbaseMaturity = 100;                                          // https://github.com/TurboStake/TurboStake/blob/master/src/main.h L41

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916;
        consensus.nMinerConfirmationWindow = 2016;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0000000000000000000000000000000000000000000000000000000000000000");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xb9;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xe3;
        nDefaultPort = 8327;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1435547102, 1435547102, 1560399, 0x1e0fffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("00000b8ada156793cd495e09bff9c662226ed3ed3784635295c084f7242b46df"));
        assert(genesis.hashMerkleRoot == uint256S("ff8892d29487de48caf88c622e688078a6d1268245de17248c0b1377737da38c"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,51);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,26);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,179);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
                {      0, uint256S("00000b8ada156793cd495e09bff9c662226ed3ed3784635295c084f7242b46df")},
                {  50000, uint256S("5ad69c1eb198a2a3d1b83e97a76dd44832154c9ad99d7acf05f38d18616d8d42")},
                { 100000, uint256S("000000000005740e6a917a0e0bc85f0cb4b1c2b9cdc3dd5d2539ffc64f97efa3")},
                { 150000, uint256S("4fea8db19de3bf5f76bd95540a1de6e658076dbe87127efe0c48d70bbd800f55")},
                { 200000, uint256S("6b90b686ee68fb6a668e3790e5585d1917cec4c0f4269f0423ce96f33fe63f86")},
                { 300000, uint256S("65e14399cebb7100e41c6580d7840cf872f449410a842237e84c4d4c3ce7265e")},
                { 400000, uint256S("eb33b21d49e019ff172d3ba0ff5d5dddfb65f45cd82fd5a3fecbdfd1b3cce66d")},
                { 500000, uint256S("09e192015318d2a0c926f69a76db59c94f2b923be4695cd468cf03fe3997c277")}
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 0000000000000000002d6cca6761c99b3c2e936f9a0e304b7c7651a993f461de (height 506081).
            1516903077, // * UNIX timestamp of last known number of transactions
            295363220,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.5         // * estimated number of transactions per second after that timestamp
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
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
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
