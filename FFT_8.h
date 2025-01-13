#include <hls_stream.h>
#include <ap_int.h>
#include <complex>
#include <cmath>

#define FFT_SIZE 8  // FFT size is fixed to 8 points
#define PI 3.14159265358979323846

// Define data types for FFT
typedef std::complex<float> Complex;

// Define AXI-stream compatible structure
struct axis_data {
    Complex data;
    ap_uint<1> last;
};

// Function prototype
void fft_8pt(hls::stream<axis_data> &in_stream, hls::stream<axis_data> &out_stream);
