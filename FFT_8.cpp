#include "FFT_8.h"

const int rev8[FFT_SIZE] = {0, 4, 2, 6, 1, 5, 3, 7};

// Precomputed twiddle factors
const Complex W[FFT_SIZE / 2] = {
    Complex(1.0, 0.0),
    Complex(0.7071067811865476, -0.7071067811865475),
    Complex(0.0, -1.0),
    Complex(-0.7071067811865476, -0.7071067811865475)
};

// Bit reversal function
void bit_reverse(Complex dataIn[FFT_SIZE], Complex dataOut[FFT_SIZE]) {
    for (int i = 0; i < FFT_SIZE; i++) {
        dataOut[i] = dataIn[rev8[i]];
    }
}

// Stage 1: Butterfly computations for pairs (stride = 4)
void stage1(Complex FFT_input[FFT_SIZE], Complex temp1[FFT_SIZE]) {
    for (int i = 0; i < FFT_SIZE; i += 2) {
        #pragma HLS PIPELINE
        temp1[i] = FFT_input[i] + FFT_input[i + 1];
        temp1[i + 1] = FFT_input[i] - FFT_input[i + 1];
    }
}

// Stage 2: Butterfly computations for groups of 2 (stride = 2)
void stage2(Complex temp1[FFT_SIZE], Complex temp2[FFT_SIZE]) {
    for (int i = 0; i < FFT_SIZE; i += 4) {
        for (int j = 0; j < 2; ++j) {
            #pragma HLS PIPELINE
            temp2[i + j] = temp1[i + j] + W[2 * j] * temp1[i + j + 2];
            temp2[i + 2 + j] = temp1[i + j] - W[2 * j] * temp1[i + j + 2];
        }
    }
}

// Stage 3: Final butterfly computations for all points (stride = 1)
void stage3(Complex temp2[FFT_SIZE], Complex FFT_output[FFT_SIZE]) {
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        #pragma HLS PIPELINE
        FFT_output[i] = temp2[i] + W[i] * temp2[i + 4];
        FFT_output[i + 4] = temp2[i] - W[i] * temp2[i + 4];
    }
}
void fft_8pt(hls::stream<axis_data> &in_stream, hls::stream<axis_data> &out_stream) {
	#pragma HLS INTERFACE axis register both port=in_stream
	#pragma HLS INTERFACE axis register both port=out_stream
	#pragma HLS INTERFACE ap_ctrl_none port=return

	// Local arrays for input and output
	Complex buffer[FFT_SIZE];
	Complex temp1[FFT_SIZE];
	Complex temp2[FFT_SIZE];
	axis_data local_stream;

	// Read input stream into buffer
	for (int i = 0; i < FFT_SIZE; i++) {
		#pragma HLS PIPELINE
		local_stream = in_stream.read();
		buffer[i] = local_stream.data;
	}

	// Perform bit reversal
	Complex reversed_buffer[FFT_SIZE];
	bit_reverse(buffer, reversed_buffer);

	// Perform Stage 1
	stage1(reversed_buffer, temp1);

	// Perform Stage 2
	stage2(temp1, temp2);

	// Perform Stage 3
	Complex FFT_output[FFT_SIZE];
	stage3(temp2, FFT_output);

	// Write output buffer to the stream
	for (int i = 0; i < FFT_SIZE; i++) {
		#pragma HLS PIPELINE
		local_stream.data = FFT_output[i];
		local_stream.last = (i == FFT_SIZE - 1);
		out_stream.write(local_stream);
	}
}
