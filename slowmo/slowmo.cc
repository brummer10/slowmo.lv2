// generated from file 'faust/slowmo.dsp' by dsp2cc:
// Code generated with Faust 0.9.73 (http://faust.grame.fr)

#include <cmath>
#include <algorithm>

#define FAUSTFLOAT float

namespace slowmo {

class Dsp {
private:
	uint32_t fSamplingFreq;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT	*fslider0_;
	double 	fConst0;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT	*fslider1_;
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT	*fslider2_;
	double 	fRec0[2];
	int 	iRec1[2];
	double 	fRec2[2];
	double 	faRec0[2];

public:
	void connect(uint32_t port,void* data);
	void del_instance(Dsp *p);
	void clear_state_f();
	void init(uint32_t samplingFreq);
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
	for (int i=0; i<2; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) iRec1[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<2; i++) faRec0[i] = 0;
}

inline void Dsp::init(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = (1e+05 / double(std::min<double>(192000, std::max<double>(1, fSamplingFreq))));
	clear_state_f();
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fslider0 (*fslider0_)
#define fslider1 (*fslider1_)
#define fslider2 (*fslider2_)
	double 	fSlow0 = (0.01 * (fConst0 / double(fslider0)));
	double 	fSlow1 = (0.01 * (0 - (fConst0 / double(fslider1))));
	double 	fSlow2 = (0.01 * double(fslider2));
	for (int i=0; i<count; i++) {
		double fTemp0 = (double)input0[i];
		double fTemp1 = fabs(fTemp0);
		int iTemp2 = int((iRec1[1] < 8));
		fRec0[0] = ((iTemp2)?std::max(fRec0[1], fTemp1):fTemp1);
		iRec1[0] = ((iTemp2)?(1 + iRec1[1]):1);
		fRec2[0] = ((iTemp2)?fRec2[1]:fRec0[1]);
		((int((fRec2[0] > fSlow2)))?faRec0[0] = std::min((double)1, ((faRec0[1]) + fSlow0)): faRec0[0] = std::max((double)0, ((faRec0[1]) + fSlow1)));
		output0[i] += (FAUSTFLOAT)(fTemp0 * faRec0[0]);
		// post processing
		fRec2[1] = fRec2[0];
		iRec1[1] = iRec1[0];
		fRec0[1] = fRec0[0];
		faRec0[1] = faRec0[0];
	}
#undef fslider0
#undef fslider1
#undef fslider2
}

void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case DOWNTIME: 
		fslider1_ = (float*)data; // , 5.0, 0.0, 1e+03, 0.01 
		break;
	case TRESHOLD: 
		fslider2_ = (float*)data; // , 1.0, 0.0, 1e+01, 0.01 
		break;
	case UPTIME: 
		fslider0_ = (float*)data; // , 1e+02, 0.0, 1e+03, 0.01 
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
	delete static_cast<Dsp*>(p);
}


} // end namespace slowmo
