// generate_uuidv7_xoshiro256.cpp
#include <arken/base>
#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <sys/random.h>
#include <time.h>

namespace {

struct Xoshiro256pp {
    uint64_t s[4];
    static inline uint64_t rotl(uint64_t x,int k){return (x<<k)|(x>>(64-k));}
    Xoshiro256pp(){
        if(getrandom(s,sizeof(s),0)!=(ssize_t)sizeof(s)){
            uint64_t t=(uint64_t)clock();
            s[0]=t; s[1]=t^0x9e3779b97f4a7c15ULL; s[2]=~t; s[3]=t<<1;
        }
        auto h=std::hash<std::thread::id>{}(std::this_thread::get_id());
        s[0]^=h; s[1]+=rotl(h,17); s[2]^=rotl(h,33); s[3]+=rotl(h,51);
    }
    inline uint64_t next(){
        uint64_t r=rotl(s[0]+s[3],23)+s[0];
        uint64_t t=s[1]<<17;
        s[2]^=s[0]; s[3]^=s[1]; s[1]^=s[2]; s[0]^=s[3];
        s[2]^=t; s[3]=rotl(s[3],45);
        return r;
    }
};
thread_local Xoshiro256pp rng;
thread_local uint64_t last_ms=0;
thread_local uint16_t seq=0;
constexpr char lut[]="0123456789abcdef";
inline uint64_t now_ms(){
    timespec ts; clock_gettime(CLOCK_REALTIME_COARSE,&ts);
    return uint64_t(ts.tv_sec)*1000ull+ts.tv_nsec/1000000ull;
}

}
std::string generate_uuidv7_xoshiro256(){
    uint8_t u[16];
    uint64_t ms=now_ms();
    if(ms==last_ms) ++seq; else {last_ms=ms; seq=rng.next()&0x0fff;}
    u[0]=ms>>40;u[1]=ms>>32;u[2]=ms>>24;u[3]=ms>>16;u[4]=ms>>8;u[5]=ms;
    uint64_t r=rng.next();
    u[6]=0x70|((seq>>8)&0x0f);
    u[7]=seq&0xff;
    u[8]=0x80|((r>>58)&0x3f);
    u[9]=r>>50;u[10]=r>>42;u[11]=r>>34;u[12]=r>>26;u[13]=r>>18;u[14]=r>>10;u[15]=r>>2;
    char out[36]; int p=0;
    for(int i=0;i<16;i++){ if(i==4||i==6||i==8||i==10) out[p++]='-'; out[p++]=lut[u[i]>>4]; out[p++]=lut[u[i]&15];}
    return std::string(out,36);
}

string os::uuid()
{
  return string(std::move(generate_uuidv7_xoshiro256()));
}
