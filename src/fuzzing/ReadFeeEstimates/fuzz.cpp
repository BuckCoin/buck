#include "txmempool.h"

#ifdef FUZZ_WITH_AFL

int main (int argc, char *argv[]) {
    CFeeRate rate;
    CTxMemPool mempool(rate);
    CAutoFile est_filein(fopen(argv[1], "rb"), SER_DISK, CLIENT_VERSION);

    if (mempool.ReadFeeEstimates(est_filein)) {
        return 0;
    } else {
        return -1;
    }
}

#endif

#ifdef FUZZ_WITH_LIBFUZZER

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    CFeeRate rate;
    CTxMemPool mempool(rate);
    CAutoFile est_filein(fmemopen(Data, Size, "rb"), SER_DISK, CLIENT_VERSION);

    if (mempool.ReadFeeEstimates(est_filein)) {
        return 0;
    } else {
        return -1;
    }
}


#endif
