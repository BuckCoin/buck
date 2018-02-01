// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "base58.h"

using namespace std;

#include "chainparamsseeds.h"

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

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "buck";
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidySlowStartInterval = 2;
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 13;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 34;
        consensus.nPowMaxAdjustUp = 34;
        consensus.nPowTargetSpacing = 2.5 * 60;
        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x24;
        pchMessageStart[1] = 0xe9;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0x64;
        vAlertPubKey = ParseHex("04a0c33e275d798fd213f48f7f1c12aded906db7d9a44138ab1da769b089dd2e1feffdaff66b1d073b89975f9c02be3d1053667d05411ab6c7d270f990a0b9a026");
        nDefaultPort = 5749;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
         * >>> from pyblake2 import blake2s
         * >>> 'Buck' + blake2s(b''Buck - The future of cryptocurrency is here now. BTC #504842 - 00000000000000000044055608d42d7a7a9bfc9de48f83c71ff1a1bd87cd4567').hexdigest()
         */
        const char* pszTimestamp = "Buck75fbaf32cef738a69530571f5e1b1e280bf6541a97596774bbcf94e18e941bb5";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime    = 1516390573;
        genesis.nBits    = 0x1f07ffff;
        genesis.nNonce   = uint256S("0x0000000000000000000000000000000000000000000000000000000000000c4a");
        genesis.nSolution = ParseHex("001ad13054574bf76543207dd77ef1e42fdaa18c3504c87de2c8f1ad758ff575253cca738ef9e25fb6991f94519400cc46e5b15236bd6074c0ca70b75da85b3f4db2151bf6b725f681c7b26d53fa8e776e5d1be2012076d2a909b993ed52b1af6c2b4041761d8d637305632163481010d9137eb0ca9533aeaa2836d1d2e707a692a1c6617e87a267920aebbe6269e78a7290914822328b7e960268da6a54cd15d3d4598994d4b133001f10da87df5301d6ee85fc654c06365c9a56d65617b20e11956bf055ac8ab48b4b7708061c479b5ff92572b2e265e37045ab78b619a8bf7551e72e7066bf532e942b8b171dbdd0386660c3bad43acbb9ffa2490a317c4aa5e551c95da954e43e347d3e26adb4dcfc32481e9bf417d49ae96f03c061b3f8424968b6fea60ca8acc7c1301661e362917b6af172c4e8730bc1b63b0f5678c4b6c4b9f6c813cabcdca445100068a1100648d20180b90a83c8d944d3056c5125812530cfb20ec55daa4ae536bfba9f2286f04ba504c1706dc9d60d3724c3faab7e4f62b804f9bafd18c9bbe374f7130db382c9ef296ad5fba4f27964f7d358bdc14b6a3a1a3e7a23161c0d61bb89f3e3f652912de8f776117f22f4cadd7f8c0db957e642fa38d44b195d02cefd541b0b91ca80df8c27037e03595e44244a06d2d306c940f31a8385d0c03dafc607932de708cea4453534eb0aee3ec7af55a335e030334b9c30954d4cbe6c98c00e4f1efcb00e6f4712aac398e8fdcd31f2b298120d0ef4266a6b1f076db88073b66476fe51082d3f7f6a42f9f3af92f057e3f4aed7ff96d13de361a29f65e116adee9881ee8639ee74734e07f3aca1624f2d16e627f7c1f8e60a619bc9dee2bc756ded41dd23d715f81882741bc9ccdff0d1e3952df9b536ae7cea1da3902a40faf1250b2efa6dd03437afe3dcefb9ff5eb18e00fcc2f458aa9467d886b2e093f2cbcaaf9556b2410972bab0e1e01d69a0a862d439639b1dd1490fbc510e289931758a5e7bd298231784e28ed9880e523b8e2827c37b55dafde52925a4d9b6f93505d5cf1928830cef2a75f404309d4c2960e7eefbe141e0cdb61afb51ea4dcf69dd54ddb5d8b624daf45aaaab635684441f40e7f2209d7d130bd7b201802af2572d337b6b0b21fe7c72b61e7c870a2c8555c53fd08eb95efd2571044770fad71a6ef79c56414164ef1e9069ad563dd135d904ec4c4ed7d5e118c5d1ef368de9f74e3b1a7217febf5c2059c8e5fe6852365493d9329a21f9f8fe2df38a9eda6e9a497756f3d36d2c305e68bc7684ee0ad155deaa4417d3ba0711489cd10f69393d96938b0c3bdb33a44a329aaefef22111ca8571fd42b5dc6d0b813cf034d98af7807921680629ff2a9a7159b9bb1495b21d60c83f6d3217f3ca35e596ee0052f61b7b047e6714a1ebc4c3a157c04ac64fd621a2455f9c571042cc6d17acb157ed64560070cf7d6e79baf481760820c65ea16b3b4d644478b5a2dbeb16723e7ad30d0f2d32f7808b5798ba7c411a56f7c714a2337a282c051fb8537e1210834c9576d694de5061ee69f6cc4223964cdcf80a248ab74ef70116bd00bf80badf37071f4ab6bb640c45291274836e2944286ab26cd9db082c36a70b000f8a33b99de66ae3736a7e6f2e786df5066f9581c61a7984fa1460908af41be8f9f8f6bced35f4c702c096e8e790fc337eb64b86d8f2786f10da0f834768b5e3b9cd8d45d373ab58d3b786c11f72f02f047cedbed9760137dd43dcddd0e06717635c7d4a10bf6fef6ba7eb775c549511bcd7fc41f7fb5194741e91b7bd2ea3b8c9e38bd38c2779782258ed344efe111691fe6f0616232857e1723fe6d204f479b5afa5535dd2b1e5d523779596e563b9de9a460c957455ae");

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x942b9449756c3f4d949c50358358de6a4f14f7d8b2b8bfd33ba5b7dcc0b9373b"));
        assert(genesis.hashMerkleRoot == uint256S("0x8e930711ce12fa8710b64dd2e11a013683144a957c23b17696e1d6e85445005a"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // use name as: echo -n hostname | sha256sum


        // guarantees the first 2 characters, when base58 encoded, are "t1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xB8};
        // guarantees the first 2 characters, when base58 encoded, are "t3"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBD};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x942b9449756c3f4d949c50358358de6a4f14f7d8b2b8bfd33ba5b7dcc0b9373b"))
            ( 2500, uint256S("0x0002f370170188a0649583b93a5af30e30022936ae5e43b15a39c1f4a7547aa2"))
            ( 6000, uint256S("0x00000078dc22f3b307b80a74dd1b001fda29f0d23661585506053c8bce4c4162")),
            1517275831,     // * UNIX timestamp of last checkpoint block
            11841,         // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            1136.736        // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
        };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "TBK";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0x1a;
        pchMessageStart[2] = 0xf9;
        pchMessageStart[3] = 0xbf;
        vAlertPubKey = ParseHex("04141aa9d685d925bb572b8e5ef63e299f64bddeea523c48c4d21433b47a1baffaff25a8ac40e5c6f3eac4a00c0ff45d9535812378c0acefd9706cde8fa0d57ffb");
        nDefaultPort = 15749;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1516390573;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x000000000000000000000000000000000000000000000000000000000000001e");
        genesis.nSolution = ParseHex("001366570489cac750d596db6b771181c9e0b71f8e44700ae9e9969623634e66b78c6652b2504158f2d103c09ae92d544229407c03e6c6ab6139e9b77c23021259312ebc652c658671636fa35a90da3ef5dc83560029c2c8de44fecf460c100cd858d0203b3a70ce6707bada5cea2fbc43e6a472fd44213d18c59ae83a1216a8bf7cf0e6f95b41fd24f3874d156981c33d44e438f5be67c1d4fc95a60c8545cee73fb310a83ab8c707430dd5dfc6043fea0b223a2a27ebea407bda1cce73601697c21f98c130b567a086f0177e03865462770f7255e13d354347b7c4c3b1966f2e4153d83abd291400a765ffb1a0099543e28c8d75d2d1f598fde7ca0f3d94ae83614ee7f456830304cd47ae33d499f1aa3cd4454aab6861e75d8a148f8cad0309fee8b5c5bb29f9cbfb0b51a73126b355f094ead649c3a19c8fb75397acbe2cd83c70eb66c62b0037922dd0c01cbed700910f2c1a0f9d0fbbd19802b66ea66ecccc9a0521175233b2fa09a89ac46cf3641ecbf7ea71ea383930043bc493d2dae421ae74e0640d2bf8f901bb0abb3b0fa96e28059a2115c18b73d5e353ee3e5f345ac90e0208da2e3d833e6bc266e8d4886bc1531528dbc7fe60b5ae03bba41e637e5c871a05784db67c4f1e163a1b37f5916dd93834f043120876122f509b0377325d39efeb785a7725dfd2e9c61365620612522adea6cf067b7a83c305d4dd85c4135f0d3256c19bb59b70df3716eb01a538d653d9a77a4f04f5245732b17a20b919ad1a953796659cfaa985548a3c53fa156f3d9f0927808e1bb54f41ead8f826a6754c6179fa511a190109bdf9cab871bc4bd16020a6e556a9785b7cc7d38420d814a24e8fe387be54633b3f479f2ec7c23da8061b47ea2d5928fca56fc943e48749081e664ab4acc61f805340ea216fb5f624a8fdbae1f876652f3cc1a9012c9dad988a11c8721276ce25c59795c86af86170017a7d5a9f216f6f27a3e2979c7c944cddfcee4551384ecc33967e0041f315797d43dd5153a9317d72093fd87bc5c9128666d6f5dcce147e821b50977c31ba08e3a2c4d04e9cb97c9101777f5edb7ad27b16ad8611e336e85b5a229356cd655bc9cc870724341c90720b4a350414a259b75fc6d272a34ed41b6b85df02c317270fa372464600fea7e3781bee90bf5c7f3f613a0a377139dcc631c0434f521cd3bef5de45b036fe545306cf182cd4e6295c9715920ab58b9e73c51521503246f5b55892cfc0fe694854847dd46e29a23cf40e4cbde773af9d339991ae3505b7bfdfaa5971b8abe30c8e119004c79e9f8878662a29c631f5ca84eec9c43fad5df37ee511f733ddb4f8304c27c1f131bcad7813b69443c2e1fde1ffdbf28070e1bf5771a65eb349216d64d158acace9c24ae2c591e9b403b79bbf87ee0215962a7cd9326769f672f68532326d4da9cc27521b06c4d0a8926482d748441c423fb6a5a6872dac430b4cded0e155c50f5d2f227401414351354e0ce03c0d17be31bec987f341e614c8b8555ec9dbaf9f423602e4628afe051182c6a8295a4ae3192f7bd71f963f23852b0b4215f76bec7a723bd9e41a2e5693f5c06c0f88f1fb03c837cdae18d5b66274c32702857d662f21ae56945c5efc27612fd412a253076d9d86dfa54a041f31b34e82c93583edd382f4298b70f49429c7010b4a762ec6cfe18d75ad2259d2c289ed9d5f3166940a53785d59a7aa0deb56432c09f81bc69b1b9b204a17587d7e26c9b8a2e936760977f479ba4dd61929ea16171c308eec37e1fbedd16dd669e5f928e45995e32bf9c1c006a9ee8d581345b527c56f5fa6947e21da1a5b0deb941116eae3e4254d34e4dbd6b86bd721b86e8a3dd64c5f898afefa072a967c22fdff9db1c820");
        consensus.hashGenesisBlock = genesis.GetHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x001750e60b9147bbf744fd90bcba4bb8407cc02f3dcef95c81ed74a426c8984e"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("rotorproject.org", "test-dnsseed.rotorproject.org")); // Zclassic

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
        };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 500000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0x3f;
        pchMessageStart[3] = 0x5f;
        nMaxTipAge = 24 * 60 * 60;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
        genesis.nTime = 1482971059;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000009");
        genesis.nSolution = ParseHex("05ffd6ad016271ade20cfce093959c3addb2079629f9f123c52ef920caa316531af5af3f");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 11989;
        //assert(consensus.hashGenesisBlock == uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        //vFoundersRewardAddress = { "t2FwcEhFdNXuFMv1tcYwaBJtYVtMj8b1uTg" };
    vFoundersRewardAddress = { };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

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
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CBitcoinAddress address(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(address.IsValid());
    assert(address.IsScript());
    CScriptID scriptID = get<CScriptID>(address.Get()); // Get() returns a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}