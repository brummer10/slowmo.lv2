
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <cstring>
#include <unistd.h>
///////////////////////// DENORMAL PROTECTION WITH SSE /////////////////

#ifdef NOSSE
#undef __SSE__
#endif

#ifdef __SSE__
#include <immintrin.h>
#ifndef _IMMINTRIN_H_INCLUDED
#include <fxsrintrin.h>
#endif
/* On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
   flags to avoid costly denormals */
#ifdef __SSE3__
#ifndef _PMMINTRIN_H_INCLUDED
#include <pmmintrin.h>
#endif
#else
#ifndef _XMMINTRIN_H_INCLUDED
#include <xmmintrin.h>
#endif
#endif //__SSE3__

#endif //__SSE__


#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
///////////////////////// MACRO SUPPORT ////////////////////////////////

#define PLUGIN_URI "urn:brummer:slowmo"

using std::min;
using std::max;

typedef enum
{
   input0,
   output0,
   bypass,
   DOWNTIME,  // , 5.0, 0.0, 1e+03, 0.01 
   TRESHOLD,  // , 1.0, 0.0, 1e+01, 0.01
   UPTIME, // , 1e+02, 0.0, 1e+03, 0.01
   VOLUME, // , 0.0, -2e+01, 6.0, 0.1 
   BPM, // , 1.2e+02, 24.0, 3.6e+02, 1.0 
   GAIN, // , 0.0, -2e+01, 2e+01, 0.1 
   FEEDBACK, // , 0.5, 0.0, 1.0, 0.01 
} PortIndex;

#include "Bandsplit.cc"
#include "slowmo.cc"
#include "Volume.cc"
#include "Delay.cc"

////////////////////////////// PLUG-IN CLASS ///////////////////////////

namespace slowmo {

class DenormalProtection
{
private:
#if defined (__SSE__) || (__arm64__) || defined (__aarch64__)
  uint32_t  mxcsr_mask;
  uint32_t  mxcsr;
  uint32_t  old_mxcsr;
#endif

public:
  inline void set_() {
#if defined (__arm64__) || defined (__aarch64__)
    asm volatile("mrs %0, fpcr" : "=r" (old_mxcsr));
    mxcsr = old_mxcsr;
    asm volatile("msr fpcr, %0" : : "ri" (mxcsr | 1 << 24));
#elif defined (__SSE__)
    old_mxcsr = _mm_getcsr();
    mxcsr = old_mxcsr;
    _mm_setcsr((mxcsr | _MM_DENORMALS_ZERO_MASK | _MM_FLUSH_ZERO_MASK) & mxcsr_mask);
#endif
  };
  inline void reset_() {
#if defined (__arm64__) || defined (__aarch64__)
    asm volatile("msr fpcr, %0" : : "ri" (old_mxcsr));
#elif defined (__SSE__)
    _mm_setcsr(old_mxcsr);
#endif
  };

  inline DenormalProtection()
  {
#ifdef __SSE__
    mxcsr_mask = 0xffbf; // Default MXCSR mask
    mxcsr      = 0;
#if defined(__x86_64__) || defined(_M_X64)
    uint8_t fxsave[512] __attribute__ ((aligned (16))); // Structure for storing FPU state with FXSAVE command

    memset(fxsave, 0, sizeof(fxsave));
    __builtin_ia32_fxsave(&fxsave);
    uint32_t mask = *(reinterpret_cast<uint32_t *>(&fxsave[0x1c])); // Obtain the MXCSR mask from FXSAVE structure
    if (mask != 0)
        mxcsr_mask = mask;
#endif
#endif
  };

  inline ~DenormalProtection() {};
};

class Xslowmo
{
private:
    float* input0;
    float* output0;
    float* bypass;
    float bypass_;
    // bypass ramping
    bool needs_ramp_down;
    bool needs_ramp_up;
    float ramp_down;
    float ramp_up;
    float ramp_up_step;
    float ramp_down_step;
    bool bypassed;

    bandsplit::Dsp* plugin_mel;
    slowmo::Dsp* plugin1;
    slowmo::Dsp* plugin2;
    slowmo::Dsp* plugin3;
    slowmo::Dsp* plugin4;
    slowmo::Dsp* plugin5;
    slowmo::Dsp* plugin6;
    slowmo::Dsp* plugin7;
    slowmo::Dsp* plugin8;
    slowmo::Dsp* plugin9;
    slowmo::Dsp* plugin10;
    slowmo::Dsp* plugin11;
    slowmo::Dsp* plugin12;
    slowmo::Dsp* plugin13;
    slowmo::Dsp* plugin14;
    slowmo::Dsp* plugin15;
    slowmo::Dsp* plugin16;
    slowmo::Dsp* plugin17;
    slowmo::Dsp* plugin18;
    gain::Dsp* plugin_g;
    delay::Dsp* plugin;

    DenormalProtection MXCSR;
    // private functions
    inline void run_dsp_(uint32_t n_samples);
    inline void connect_(uint32_t port,void* data);
    inline void init_dsp_(uint32_t rate);
    inline void connect_all__ports(uint32_t port, void* data);
    inline void activate_f();
    inline void clean_up();
    inline void deactivate_f();
public:
    // LV2 Descriptor
    static const LV2_Descriptor descriptor;
    // static wrapper to private functions
    static void deactivate(LV2_Handle instance);
    static void cleanup(LV2_Handle instance);
    static void run(LV2_Handle instance, uint32_t n_samples);
    static void activate(LV2_Handle instance);
    static void connect_port(LV2_Handle instance, uint32_t port, void* data);
    static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                double rate, const char* bundle_path,
                                const LV2_Feature* const* features);
    Xslowmo();
    ~Xslowmo();
};

// constructor
Xslowmo::Xslowmo() :

    input0(NULL),
    output0(NULL),
    bypass(NULL),
    bypass_(2),
    needs_ramp_down(false),
    needs_ramp_up(false),
    bypassed(false),
    plugin_mel(bandsplit::plugin()),
    plugin1(slowmo::plugin()),
    plugin2(slowmo::plugin()),
    plugin3(slowmo::plugin()),
    plugin4(slowmo::plugin()),
    plugin5(slowmo::plugin()),
    plugin6(slowmo::plugin()),
    plugin7(slowmo::plugin()),
    plugin8(slowmo::plugin()),
    plugin9(slowmo::plugin()),
    plugin10(slowmo::plugin()),
    plugin11(slowmo::plugin()),
    plugin12(slowmo::plugin()),
    plugin13(slowmo::plugin()),
    plugin14(slowmo::plugin()),
    plugin15(slowmo::plugin()),
    plugin16(slowmo::plugin()),
    plugin17(slowmo::plugin()),
    plugin18(slowmo::plugin()),
    plugin_g(gain::plugin()),
    plugin(delay::plugin()) {};

// destructor
Xslowmo::~Xslowmo() {
    plugin_mel->del_instance(plugin_mel);
    plugin1->del_instance(plugin1);
    plugin2->del_instance(plugin2);
    plugin3->del_instance(plugin3);
    plugin4->del_instance(plugin4);
    plugin5->del_instance(plugin5);
    plugin6->del_instance(plugin6);
    plugin7->del_instance(plugin7);
    plugin8->del_instance(plugin8);
    plugin9->del_instance(plugin9);
    plugin10->del_instance(plugin10);
    plugin11->del_instance(plugin11);
    plugin12->del_instance(plugin12);
    plugin13->del_instance(plugin13);
    plugin14->del_instance(plugin14);
    plugin15->del_instance(plugin15);
    plugin16->del_instance(plugin16);
    plugin17->del_instance(plugin17);
    plugin18->del_instance(plugin18);
    plugin_g->del_instance(plugin_g);
    plugin->del_instance(plugin);
};

///////////////////////// PRIVATE CLASS  FUNCTIONS /////////////////////

void Xslowmo::init_dsp_(uint32_t rate)
{
    plugin_mel->init(rate);
    plugin1->init(rate);
    plugin2->init(rate);
    plugin3->init(rate);
    plugin4->init(rate);
    plugin5->init(rate);
    plugin6->init(rate);
    plugin7->init(rate);
    plugin8->init(rate);
    plugin9->init(rate);
    plugin10->init(rate);
    plugin11->init(rate);
    plugin12->init(rate);
    plugin13->init(rate);
    plugin14->init(rate);
    plugin15->init(rate);
    plugin16->init(rate);
    plugin17->init(rate);
    plugin18->init(rate);
    plugin_g->init(rate);
    plugin->init(rate);
    // set values for internal ramping
    ramp_down_step = 32 * (256 * rate) / 48000; 
    ramp_up_step = ramp_down_step;
    ramp_down = ramp_down_step;
    ramp_up = 0.0;
}

// connect the Ports used by the plug-in class
void Xslowmo::connect_(uint32_t port,void* data)
{
    switch ((PortIndex)port)
    {
        case 0:
            input0 = static_cast<float*>(data);
            break;
        case 1:
            output0 = static_cast<float*>(data);
            break;
        case 2:
            bypass = static_cast<float*>(data);
            break;
        default:
            break;
    }
}

void Xslowmo::activate_f()
{
    // allocate the internal DSP mem
}

void Xslowmo::clean_up()
{
    // delete the internal DSP mem
}

void Xslowmo::deactivate_f()
{
    // delete the internal DSP mem
}

void Xslowmo::run_dsp_(uint32_t n_samples)
{
    if(n_samples<1) return;
    MXCSR.set_();

    float output00[n_samples];
    memset(output00, 0, n_samples*sizeof(output00[0]));

    float output1[n_samples];
    float output2[n_samples];
    float output3[n_samples];
    float output4[n_samples];
    float output5[n_samples];
    float output6[n_samples];
    float output7[n_samples];
    float output8[n_samples];
    float output9[n_samples];
    float output10[n_samples];
    float output11[n_samples];
    float output12[n_samples];
    float output13[n_samples];
    float output14[n_samples];
    float output15[n_samples];
    float output16[n_samples];
    float output17[n_samples];
    float output18[n_samples];

    // do inplace processing on default
    if(output0 != input0)
        memcpy(output0, input0, n_samples*sizeof(float));

    float buf0[n_samples];
    // check if bypass is pressed
    if (bypass_ != static_cast<uint32_t>(*(bypass))) {
        bypass_ = static_cast<uint32_t>(*(bypass));
        if (!bypass_) {
            needs_ramp_down = true;
            needs_ramp_up = false;
        } else {
            needs_ramp_down = false;
            needs_ramp_up = true;
            bypassed = false;
        }
    }

    if (needs_ramp_down || needs_ramp_up) {
         memcpy(buf0, input0, n_samples*sizeof(float));
    }
    if (!bypassed) {
        plugin_mel->compute(n_samples, output0, output1, output2, output3, output4, output5, output6, output7, output8,
            output9, output10, output11, output12, output13, output14, output15, output16, output17, output18);
        plugin1->compute(n_samples, output1, output00);
        plugin2->compute(n_samples, output2, output00);
        plugin3->compute(n_samples, output3, output00);
        plugin4->compute(n_samples, output4, output00);
        plugin5->compute(n_samples, output5, output00);
        plugin6->compute(n_samples, output6, output00);
        plugin7->compute(n_samples, output7, output00);
        plugin8->compute(n_samples, output8, output00);
        plugin9->compute(n_samples, output9, output00);
        plugin10->compute(n_samples, output10, output00);
        plugin11->compute(n_samples, output11, output00);
        plugin12->compute(n_samples, output12, output00);
        plugin13->compute(n_samples, output13, output00);
        plugin14->compute(n_samples, output14, output00);
        plugin15->compute(n_samples, output15, output00);
        plugin16->compute(n_samples, output16, output00);
        plugin17->compute(n_samples, output17, output00);
        plugin18->compute(n_samples, output18, output00);
        plugin->compute(n_samples, output00, output00);
        plugin_g->compute(n_samples, output00, output0);
    }               

    // check if ramping is needed
    if (needs_ramp_down) {
        float fade = 0;
        for (uint32_t i=0; i<n_samples; i++) {
            if (ramp_down >= 0.0) {
                --ramp_down; 
            }
            fade = max(0.0f,ramp_down) /ramp_down_step ;
            output0[i] = output0[i] * fade + buf0[i] * (1.0 - fade);
        }
        if (ramp_down <= 0.0) {
            // when ramped down, clear buffer from dsp
            plugin1->clear_state_f();
            needs_ramp_down = false;
            bypassed = true;
            ramp_down = ramp_down_step;
            ramp_up = 0.0;
        } else {
            ramp_up = ramp_down;
        }
    } else if (needs_ramp_up) {
        float fade = 0;
        for (uint32_t i=0; i<n_samples; i++) {
            if (ramp_up < ramp_up_step) {
                ++ramp_up ;
            }
            fade = min(ramp_up_step,ramp_up) /ramp_up_step ;
            output0[i] = output0[i] * fade + buf0[i] * (1.0 - fade);
        }
        if (ramp_up >= ramp_up_step) {
            needs_ramp_up = false;
            ramp_up = 0.0;
            ramp_down = ramp_down_step;
        } else {
            ramp_down = ramp_up;
        }
    }
    MXCSR.reset_();
}

void Xslowmo::connect_all__ports(uint32_t port, void* data)
{
    // connect the Ports used by the plug-in class
    connect_(port,data); 
    plugin1->connect(port,data);
    plugin2->connect(port,data);
    plugin3->connect(port,data);
    plugin4->connect(port,data);
    plugin5->connect(port,data);
    plugin6->connect(port,data);
    plugin7->connect(port,data);
    plugin8->connect(port,data);
    plugin9->connect(port,data);
    plugin10->connect(port,data);
    plugin11->connect(port,data);
    plugin12->connect(port,data);
    plugin13->connect(port,data);
    plugin14->connect(port,data);
    plugin15->connect(port,data);
    plugin16->connect(port,data);
    plugin17->connect(port,data);
    plugin18->connect(port,data);
    plugin_g->connect(port,data);
    plugin->connect(port,data);

}

////////////////////// STATIC CLASS  FUNCTIONS  ////////////////////////

LV2_Handle 
Xslowmo::instantiate(const LV2_Descriptor* descriptor,
                            double rate, const char* bundle_path,
                            const LV2_Feature* const* features)
{
    // init the plug-in class
    Xslowmo *self = new Xslowmo();
    if (!self) {
        return NULL;
    }
    self->init_dsp_((uint32_t)rate);
    return (LV2_Handle)self;
}

void Xslowmo::connect_port(LV2_Handle instance, 
                                   uint32_t port, void* data)
{
    // connect all ports
    static_cast<Xslowmo*>(instance)->connect_all__ports(port, data);
}

void Xslowmo::activate(LV2_Handle instance)
{
    // allocate needed mem
    static_cast<Xslowmo*>(instance)->activate_f();
}

void Xslowmo::run(LV2_Handle instance, uint32_t n_samples)
{
    // run dsp
    static_cast<Xslowmo*>(instance)->run_dsp_(n_samples);
}

void Xslowmo::deactivate(LV2_Handle instance)
{
    // free allocated mem
    static_cast<Xslowmo*>(instance)->deactivate_f();
}

void Xslowmo::cleanup(LV2_Handle instance)
{
    // well, clean up after us
    Xslowmo* self = static_cast<Xslowmo*>(instance);
    self->clean_up();
    delete self;
}

const LV2_Descriptor Xslowmo::descriptor =
{
    PLUGIN_URI ,
    Xslowmo::instantiate,
    Xslowmo::connect_port,
    Xslowmo::activate,
    Xslowmo::run,
    Xslowmo::deactivate,
    Xslowmo::cleanup,
    NULL
};

} // end namespace slowmo

////////////////////////// LV2 SYMBOL EXPORT ///////////////////////////

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index)
    {
        case 0:
            return &slowmo::Xslowmo::descriptor;
        default:
            return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////
