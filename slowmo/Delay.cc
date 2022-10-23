// generated from file 'delay.dsp' by dsp2cc:
// Code generated with Faust 2.50.6 (https://faust.grame.fr)

#include <cmath>
#include <algorithm>

#define FAUSTFLOAT float

#define always_inline inline __attribute__((__always_inline__))

namespace delay {

class Dsp {
private:
	uint32_t fSampleRate;
	int IOTA0;
	double fVec0[1048576];
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT	*fHslider0_;
	double fConst1;
	double fConst2;
	double fConst3;
	double fRec1[2];
	double fRec2[2];
	double fRec3[2];
	double fRec4[2];
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT	*fVslider0_;
	double fRec5[2];
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT	*fVslider1_;
	double fRec6[2];
	double fRec0[1048576];


public:
	void connect(uint32_t port,void* data);
	void del_instance(Dsp *p);
	void clear_state_f();
	void init(uint32_t sample_rate);
	void compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0);
	Dsp();
	~Dsp();
};



Dsp::Dsp() {
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int l0 = 0; l0 < 1048576; l0 = l0 + 1) fVec0[l0] = 0.0;
	for (int l1 = 0; l1 < 2; l1 = l1 + 1) fRec1[l1] = 0.0;
	for (int l2 = 0; l2 < 2; l2 = l2 + 1) fRec2[l2] = 0.0;
	for (int l3 = 0; l3 < 2; l3 = l3 + 1) fRec3[l3] = 0.0;
	for (int l4 = 0; l4 < 2; l4 = l4 + 1) fRec4[l4] = 0.0;
	for (int l5 = 0; l5 < 2; l5 = l5 + 1) fRec5[l5] = 0.0;
	for (int l6 = 0; l6 < 2; l6 = l6 + 1) fRec6[l6] = 0.0;
	for (int l7 = 0; l7 < 1048576; l7 = l7 + 1) fRec0[l7] = 0.0;
}

inline void Dsp::init(uint32_t sample_rate)
{
	fSampleRate = sample_rate;
	double fConst0 = std::min<double>(1.92e+05, std::max<double>(1.0, double(fSampleRate)));
	fConst1 = 6e+01 * fConst0;
	fConst2 = 1e+01 / fConst0;
	fConst3 = 0.0 - fConst2;
	IOTA0 = 0;
	clear_state_f();
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fHslider0 (*fHslider0_)
#define fVslider0 (*fVslider0_)
#define fVslider1 (*fVslider1_)
	double fSlow0 = fConst1 / double(fHslider0);
	double fSlow1 = 0.0010000000000000009 * std::pow(1e+01, 0.05 * double(fVslider0));
	double fSlow2 = 0.0010000000000000009 * double(fVslider1);
	for (int i0 = 0; i0 < count; i0 = i0 + 1) {
		double fTemp0 = double(input0[i0]);
		fVec0[IOTA0 & 1048575] = fTemp0;
		double fThen1 = (((fRec2[1] == 1.0) & (fSlow0 != fRec4[1])) ? fConst3 : 0.0);
		double fThen3 = (((fRec2[1] == 0.0) & (fSlow0 != fRec3[1])) ? fConst2 : fThen1);
		double fElse3 = (((fRec2[1] > 0.0) & (fRec2[1] < 1.0)) ? fRec1[1] : 0.0);
		double fTemp1 = ((fRec1[1] != 0.0) ? fElse3 : fThen3);
		fRec1[0] = fTemp1;
		fRec2[0] = std::max<double>(0.0, std::min<double>(1.0, fRec2[1] + fTemp1));
		fRec3[0] = (((fRec2[1] >= 1.0) & (fRec4[1] != fSlow0)) ? fSlow0 : fRec3[1]);
		fRec4[0] = (((fRec2[1] <= 0.0) & (fRec3[1] != fSlow0)) ? fSlow0 : fRec4[1]);
		int iTemp2 = int(std::min<double>(524288.0, std::max<double>(0.0, fRec4[0])));
		int iTemp3 = int(std::min<double>(524288.0, std::max<double>(0.0, fRec3[0])));
		double fTemp4 = fVec0[(IOTA0 - iTemp3) & 1048575];
		fRec5[0] = fSlow1 + 0.999 * fRec5[1];
		double fTemp5 = fRec0[(IOTA0 - (iTemp3 + 1)) & 1048575];
		fRec6[0] = fSlow2 + 0.999 * fRec6[1];
		fRec0[IOTA0 & 1048575] = fRec6[0] * (fTemp5 + fRec2[0] * (fRec0[(IOTA0 - (iTemp2 + 1)) & 1048575] - fTemp5)) + fRec5[0] * (2.0 * fTemp4 + fRec2[0] * (0.0 - 2.0 * (fTemp4 - fVec0[(IOTA0 - iTemp2) & 1048575])));
		output0[i0] += FAUSTFLOAT(fRec0[IOTA0 & 1048575]);
		IOTA0 = IOTA0 + 1;
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
	}
#undef fHslider0
#undef fVslider0
#undef fVslider1
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case BPM: 
		fHslider0_ = static_cast<float*>(data); // , 1.2e+02, 24.0, 3.6e+02, 1.0 
		break;
	case FEEDBACK: 
		fVslider1_ = static_cast<float*>(data); // , 0.5, 0.0, 1.0, 0.01 
		break;
	case GAIN: 
		fVslider0_ = static_cast<float*>(data); // , 0.0, -2e+01, 2e+01, 0.1 
		break;
	default:
		break;
	}
}

Dsp *plugin() {
	return new Dsp();
}

void Dsp::del_instance(Dsp *p)
{
	delete p;
}
} // end namespace delay
