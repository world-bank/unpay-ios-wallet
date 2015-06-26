//
//  WBChainParams.h
//  UnpayWallet
//
//  Created by yezune, choi on 2015. 6. 16..
//  Copyright (c) 2015년 world bank. All rights reserved.
//

#import <Foundation/Foundation.h>

#define UNPAYCOIN_NET               1


#if UNPAYCOIN_NET

// BRPeer.h {
#if BITCOIN_TESTNET
#define BITCOIN_STANDARD_PORT       13338
#else
#define BITCOIN_STANDARD_PORT       3338
#endif

#define BITCOIN_TIMEOUT_CODE        1001

#define SERVICES_NODE_NETWORK       1 // services value indicating a node carries full blocks, not just headers
#define USER_AGENT                  [NSString stringWithFormat:@"/unpay.wallet.ios:%@/",\
NSBundle.mainBundle.infoDictionary[@"CFBundleShortVersionString"]]


#if 1

#define TWO_HOURS                   (2*60*60)
#define SECONDS_OF_SEVEN_DAYS       (7*24*60*60)

#else

#define TWO_HOURS                   (0)
#define SECONDS_OF_SEVEN_DAYS       (0)

#endif

// } BRPeer.h

// BRMerkleBlock.m {
#define MAX_TIME_DRIFT              (3*60)        // the furthest in the future a block is allowed to be timestamped
#define TARGET_TIMESPAN             (24*60*60)      // the targeted timespan between difficulty target adjustments
#define MAX_PROOF_OF_WORK           0x1e0ffff0u     // highest value for difficulty target (higher values are less difficult)
// } BRMerkleBlock.m

// BRMercleBlock.h {
#define BLOCK_DIFFICULTY_INTERVAL   (TARGET_TIMESPAN / MAX_TIME_DRIFT) //2016      // number of blocks between difficulty target adjustments
#define BLOCK_UNKNOWN_HEIGHT        INT32_MAX
// } BRMerkleBlock.h


// NSMutableData+Bitcoin.h {

#if BITCOIN_TESTNET
#define BITCOIN_MAGIC_NUMBER 0xffcae2ceu
#else
#define BITCOIN_MAGIC_NUMBER 0xbd6b0cbfu
#endif
// } NSMutableData+Bitcoin.h


// BRKey.h {

#if BITCOIN_TESTNET

// TEST NET

#define BITCOIN_PUBKEY_ADDRESS      139
#define BITCOIN_SCRIPT_ADDRESS      19
#define BITCOIN_PRIVKEY             239

#else

// LIVE NET

#define BITCOIN_PUBKEY_ADDRESS      76
#define BITCOIN_SCRIPT_ADDRESS      16
#define BITCOIN_PRIVKEY             204

#endif


#define BIP38_NOEC_PREFIX           0x0142
#define BIP38_EC_PREFIX             0x0143
#define BIP38_NOEC_FLAG             (0x80 | 0x40)
#define BIP38_COMPRESSED_FLAG       0x20
#define BIP38_LOTSEQUENCE_FLAG      0x04
#define BIP38_INVALID_FLAG          (0x10 | 0x08 | 0x02 | 0x01)
// } BRKey.h

// from BRTransaction.h {
#define TX_FEE_PER_KB               1000ULL     // standard tx fee per kb of tx size, rounded up to nearest kb
#define TX_MIN_OUTPUT_AMOUNT        (TX_FEE_PER_KB*3*(34 + 148)/1000) // no txout can be below this amount (or it won't relay)
#define TX_MAX_SIZE                 100000      // no tx can be larger than this size in bytes
#define TX_FREE_MAX_SIZE            1000        // tx must not be larger than this size in bytes without a fee
#define TX_FREE_MIN_PRIORITY        57600000ULL // tx must not have a priority below this value without a fee
#define TX_UNCONFIRMED              INT32_MAX   // block height indicating transaction is unconfirmed
#define TX_MAX_LOCK_HEIGHT          500000000u  // a lockTime below this value is a block height, otherwise a timestamp
// } BRTransaction.h

// from BRPeerManager.m

#define FIXED_PEERS                 @"FixedPeers"
#define PROTOCOL_TIMEOUT            20.0
#define MAX_CONNECT_FAILURES        20 // notify user of network problems after this many connect failures in a row
#define CHECKPOINT_COUNT            (sizeof(checkpoint_array)/sizeof(*checkpoint_array))
#define GENESIS_BLOCK_HASH          ([NSString stringWithUTF8String:checkpoint_array[0].hash].hexToData.reverse)

#if BITCOIN_TESTNET

static const struct { uint32_t height; char *hash; uint32_t timestamp; uint32_t target; } checkpoint_array[] = {
    {      0, "000003e14b723be4346c6ed7c61d46c7e6d6d83d4b1c3db38b2a38248d5a134c", 1296688602, 0x1e0ffff0u },
};

static const char *dns_seeds[] = {
    "testnet-dnsseed.unpaybank.info",
 
    //
    //    "testnet-seed.bitcoin.petertodd.org.", "testnet-seed.bluematt.me.", "testnet-seed.alexykot.me."
};



#else // main net

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they need to be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const struct { uint32_t height; char *hash; uint32_t timestamp; uint32_t target; } checkpoint_array[] = {
    {      0, "00000890f1794585e882cbb9ec24760f2293fba338eb919232ff9c4f740267f4", 1433994530, 0x1e0ffff0u },
};

static const char *dns_seeds[] = {
    "dnsseed.unpaybank.info",
    
    //    "seed.bitcoin.sipa.be.", "dnsseed.bluematt.me.", "dnsseed.bitcoin.dashjr.org.", "seed.bitcoinstats.com.",
    //    "seed.bitnodes.io."
};
// } BRPeerManager.m

// from BRPeer.m {
#define HEADER_LENGTH      24
#define MAX_MSG_LENGTH     0x02000000u
#define MAX_GETDATA_HASHES 50000
#define ENABLED_SERVICES   0     // we don't provide full blocks to remote nodes
#define PROTOCOL_VERSION   70076
#define MIN_PROTO_VERSION  70066 // peers earlier than this protocol version not supported (need v0.9 txFee relay rules)
#define LOCAL_HOST         0x7f000001u
#define ZERO_HASH          [NSMutableData dataWithLength:CC_SHA256_DIGEST_LENGTH]
#define CONNECT_TIMEOUT    3.0
// } BRPeer.m

#endif // UNPAYCOIN_NET ========================================================================================

#else  // BITCOIN_NET   ========================================================================================

// BRPeer.h {
#if BITCOIN_TESTNET
#define BITCOIN_STANDARD_PORT       18333
#else
#define BITCOIN_STANDARD_PORT       8333
#endif

#define BITCOIN_TIMEOUT_CODE        1001

#define SERVICES_NODE_NETWORK       1 // services value indicating a node carries full blocks, not just headers
#define USER_AGENT                  [NSString stringWithFormat:@"/breadwallet:%@/",\
NSBundle.mainBundle.infoDictionary[@"CFBundleShortVersionString"]]


#if 1

#define TWO_HOURS                   (2*60*60)
#define SECONDS_OF_SEVEN_DAYS       (7*24*60*60)

#else

#define TWO_HOURS                   (0)
#define SECONDS_OF_SEVEN_DAYS       (0)

#endif

// } BRPeer.h

// BRMerkleBlock.m {
#define MAX_TIME_DRIFT              (2*60*60)        // the furthest in the future a block is allowed to be timestamped
#define TARGET_TIMESPAN             (14*24*60*60)      // the targeted timespan between difficulty target adjustments
#define MAX_PROOF_OF_WORK           0x1d00ffffu     // highest value for difficulty target (higher values are less difficult)
// } BRMerkleBlock.m

// BRMercleBlock.h {
#define BLOCK_DIFFICULTY_INTERVAL   (TARGET_TIMESPAN / MAX_TIME_DRIFT) //2016      // number of blocks between difficulty target adjustments
#define BLOCK_UNKNOWN_HEIGHT        INT32_MAX
// } BRMerkleBlock.h


// NSMutableData+Bitcoin.h {

#if BITCOIN_TESTNET
#define BITCOIN_MAGIC_NUMBER 0x0709110bu
#else
#define BITCOIN_MAGIC_NUMBER 0xd9b4bef9u
#endif
// } NSMutableData+Bitcoin.h


// BRKey.h {

#if BITCOIN_TESTNET

// TEST NET

#define BITCOIN_PUBKEY_ADDRESS      111
#define BITCOIN_SCRIPT_ADDRESS      196
#define BITCOIN_PRIVKEY             239

#else

// LIVE NET

#define BITCOIN_PUBKEY_ADDRESS      0
#define BITCOIN_SCRIPT_ADDRESS      5
#define BITCOIN_PRIVKEY             128

#endif


#define BIP38_NOEC_PREFIX           0x0142
#define BIP38_EC_PREFIX             0x0143
#define BIP38_NOEC_FLAG             (0x80 | 0x40)
#define BIP38_COMPRESSED_FLAG       0x20
#define BIP38_LOTSEQUENCE_FLAG      0x04
#define BIP38_INVALID_FLAG          (0x10 | 0x08 | 0x02 | 0x01)
// } BRKey.h



// from BRTransaction.h {
#define TX_FEE_PER_KB               1000ULL     // standard tx fee per kb of tx size, rounded up to nearest kb
#define TX_MIN_OUTPUT_AMOUNT        (TX_FEE_PER_KB*3*(34 + 148)/1000) // no txout can be below this amount (or it won't relay)
#define TX_MAX_SIZE                 100000      // no tx can be larger than this size in bytes
#define TX_FREE_MAX_SIZE            1000        // tx must not be larger than this size in bytes without a fee
#define TX_FREE_MIN_PRIORITY        57600000ULL // tx must not have a priority below this value without a fee
#define TX_UNCONFIRMED              INT32_MAX   // block height indicating transaction is unconfirmed
#define TX_MAX_LOCK_HEIGHT          500000000u  // a lockTime below this value is a block height, otherwise a timestamp
// } BRTransaction.h

// from BRPeerManager.m

#define FIXED_PEERS                 @"FixedPeers"
#define PROTOCOL_TIMEOUT            20.0
#define MAX_CONNECT_FAILURES        20 // notify user of network problems after this many connect failures in a row
#define CHECKPOINT_COUNT            (sizeof(checkpoint_array)/sizeof(*checkpoint_array))
#define GENESIS_BLOCK_HASH          ([NSString stringWithUTF8String:checkpoint_array[0].hash].hexToData.reverse)

#if BITCOIN_TESTNET

static const struct { uint32_t height; char *hash; uint32_t timestamp; uint32_t target; } checkpoint_array[] = {
    {      0, "000000000933ea01ad0ee984209779baaec3ced90fa3f408719526f8d77f4943", 1296688602, 0x1d00ffffu },
    {  20160, "000000001cf5440e7c9ae69f655759b17a32aad141896defd55bb895b7cfc44e", 1345001466, 0x1c4d1756u },
    {  40320, "000000008011f56b8c92ff27fb502df5723171c5374673670ef0eee3696aee6d", 1355980158, 0x1d00ffffu },
    {  60480, "00000000130f90cda6a43048a58788c0a5c75fa3c32d38f788458eb8f6952cee", 1363746033, 0x1c1eca8au },
    {  80640, "00000000002d0a8b51a9c028918db3068f976e3373d586f08201a4449619731c", 1369042673, 0x1c011c48u },
    { 100800, "0000000000a33112f86f3f7b0aa590cb4949b84c2d9c673e9e303257b3be9000", 1376543922, 0x1c00d907u },
    { 120960, "00000000003367e56e7f08fdd13b85bbb31c5bace2f8ca2b0000904d84960d0c", 1382025703, 0x1c00df4cu },
    { 141120, "0000000007da2f551c3acd00e34cc389a4c6b6b3fad0e4e67907ad4c7ed6ab9f", 1384495076, 0x1c0ffff0u },
    { 161280, "0000000001d1b79a1aec5702aaa39bad593980dfe26799697085206ef9513486", 1388980370, 0x1c03fffcu },
    { 181440, "00000000002bb4563a0ec21dc4136b37dcd1b9d577a75a695c8dd0b861e1307e", 1392304311, 0x1b336ce6u },
    { 201600, "0000000000376bb71314321c45de3015fe958543afcbada242a3b1b072498e38", 1393813869, 0x1b602ac0u }
};

static const char *dns_seeds[] = {
    "testnet-seed.bitcoin.petertodd.org.", "testnet-seed.bluematt.me.", "testnet-seed.alexykot.me."
};

#else // main net

// blockchain checkpoints - these are also used as starting points for partial chain downloads, so they need to be at
// difficulty transition boundaries in order to verify the block difficulty at the immediately following transition
static const struct { uint32_t height; char *hash; uint32_t timestamp; uint32_t target; } checkpoint_array[] = {
    {      0, "000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f", 1231006505, 0x1d00ffffu },
    {  20160, "000000000f1aef56190aee63d33a373e6487132d522ff4cd98ccfc96566d461e", 1248481816, 0x1d00ffffu },
    {  40320, "0000000045861e169b5a961b7034f8de9e98022e7a39100dde3ae3ea240d7245", 1266191579, 0x1c654657u },
    {  60480, "000000000632e22ce73ed38f46d5b408ff1cff2cc9e10daaf437dfd655153837", 1276298786, 0x1c0eba64u },
    {  80640, "0000000000307c80b87edf9f6a0697e2f01db67e518c8a4d6065d1d859a3a659", 1284861847, 0x1b4766edu },
    { 100800, "000000000000e383d43cc471c64a9a4a46794026989ef4ff9611d5acb704e47a", 1294031411, 0x1b0404cbu },
    { 120960, "0000000000002c920cf7e4406b969ae9c807b5c4f271f490ca3de1b0770836fc", 1304131980, 0x1b0098fau },
    { 141120, "00000000000002d214e1af085eda0a780a8446698ab5c0128b6392e189886114", 1313451894, 0x1a094a86u },
    { 161280, "00000000000005911fe26209de7ff510a8306475b75ceffd434b68dc31943b99", 1326047176, 0x1a0d69d7u },
    { 181440, "00000000000000e527fc19df0992d58c12b98ef5a17544696bbba67812ef0e64", 1337883029, 0x1a0a8b5fu },
    { 201600, "00000000000003a5e28bef30ad31f1f9be706e91ae9dda54179a95c9f9cd9ad0", 1349226660, 0x1a057e08u },
    { 221760, "00000000000000fc85dd77ea5ed6020f9e333589392560b40908d3264bd1f401", 1361148470, 0x1a04985cu },
    { 241920, "00000000000000b79f259ad14635739aaf0cc48875874b6aeecc7308267b50fa", 1371418654, 0x1a00de15u },
    { 262080, "000000000000000aa77be1c33deac6b8d3b7b0757d02ce72fffddc768235d0e2", 1381070552, 0x1916b0cau },
    { 282240, "0000000000000000ef9ee7529607286669763763e0c46acfdefd8a2306de5ca8", 1390570126, 0x1901f52cu },
    { 302400, "0000000000000000472132c4daaf358acaf461ff1c3e96577a74e5ebf91bb170", 1400928750, 0x18692842u },
    { 322560, "000000000000000002df2dd9d4fe0578392e519610e341dd09025469f101cfa1", 1411680080, 0x181fb893u },
    { 342720, "00000000000000000f9cfece8494800d3dcbf9583232825da640c8703bcd27e7", 1423496415, 0x1818bb87u }
};

static const char *dns_seeds[] = {
    "seed.bitcoin.sipa.be.", "dnsseed.bluematt.me.", "dnsseed.bitcoin.dashjr.org.", "seed.bitcoinstats.com.",
    "seed.bitnodes.io."
};
#endif // BITCOIN_TESTNET

// from BRPeer.m {
#define HEADER_LENGTH      24
#define MAX_MSG_LENGTH     0x02000000u
#define MAX_GETDATA_HASHES 50000
#define ENABLED_SERVICES   0     // we don't provide full blocks to remote nodes
#define PROTOCOL_VERSION   70002
#define MIN_PROTO_VERSION  70002 // peers earlier than this protocol version not supported (need v0.9 txFee relay rules)
#define LOCAL_HOST         0x7f000001u
#define ZERO_HASH          [NSMutableData dataWithLength:CC_SHA256_DIGEST_LENGTH]
#define CONNECT_TIMEOUT    3.0
// } BRPeer.m

#endif  //UNPAYCOIN_NET

@interface WBChainParams : NSObject

@end
