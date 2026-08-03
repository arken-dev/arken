#include <arken/base>
// https://uuidv7.org
//
// Reference-quality UUIDv7 generator (C++17)
// Features:
// - RFC9562 layout (48-bit Unix ms timestamp, version 7, variant RFC4122)
// - thread_local xoshiro256++ PRNG
// - monotonic sequence within same millisecond (12 bits)
// - getrandom() seeding
// - clock_gettime()
// - no malloc except returned std::string
// - LUT-based hex formatting
//
// NOTE:
// This is intended as a strong reference implementation. A fully production-
// hardened library would also include parser, binary API, tests and benchmarks.

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <time.h>
#include <sys/random.h>
#include <thread>
#include <functional>

namespace {
struct Xoshiro256pp {
    uint64_t s[4];
    static inline uint64_t rotl(uint64_t x,int k){return (x<<k)|(x>>(64-k));}
    Xoshiro256pp(){
        getrandom(s,sizeof(s),0);
        auto h=std::hash<std::thread::id>{}(std::this_thread::get_id());
        s[0]^=h; s[1]+=rotl(h,13); s[2]^=rotl(h,29); s[3]+=rotl(h,47);
    }
    inline uint64_t next(){
        uint64_t r=rotl(s[0]+s[3],23)+s[0];
        uint64_t t=s[1]<<17;
        s[2]^=s[0]; s[3]^=s[1];
        s[1]^=s[2]; s[0]^=s[3];
        s[2]^=t; s[3]=rotl(s[3],45);
        return r;
    }
};
thread_local Xoshiro256pp rng;
thread_local uint64_t last_ms=0;
thread_local uint16_t seq=0;
constexpr char HEX[]="0123456789abcdef";
inline uint64_t unix_ms(){
    timespec ts;
    clock_gettime(CLOCK_REALTIME,&ts);
    return uint64_t(ts.tv_sec)*1000ULL+ts.tv_nsec/1000000ULL;
}
}

std::string generate_uuidv7_xoshiro256() {
    uint8_t u[16];
    uint64_t ms=unix_ms();
    if(ms==last_ms) seq=(seq+1)&0x0fff;
    else { last_ms=ms; seq=rng.next()&0x0fff; }

    u[0]=ms>>40; u[1]=ms>>32; u[2]=ms>>24;
    u[3]=ms>>16; u[4]=ms>>8;  u[5]=ms;

    uint64_t r1=rng.next(), r2=rng.next();

    u[6]=0x70|((seq>>8)&0x0f);
    u[7]=seq&0xff;

    u[8]=0x80|((r1>>58)&0x3f);
    u[9]=r1>>50; u[10]=r1>>42; u[11]=r1>>34;
    u[12]=r1>>26; u[13]=r1>>18; u[14]=r1>>10; u[15]=r2>>56;

    char out[36];
    int p=0;
    for(int i=0;i<16;i++){
        if(i==4||i==6||i==8||i==10) out[p++]='-';
        out[p++]=HEX[u[i]>>4];
        out[p++]=HEX[u[i]&0x0f];
    }
    return std::string(out,36);
}

string os::uuid()
{
  return string(std::move(generate_uuidv7_xoshiro256()));
}
