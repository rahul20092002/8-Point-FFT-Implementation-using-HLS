#include "FFT_8.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

int main() {
    hls::stream<axis_data> in_stream, out_stream;
    axis_data local_stream;
    Complex input_signal[FFT_SIZE];
    Complex output_hw[FFT_SIZE];
    Complex output_sw[FFT_SIZE];

    // Generate random input signal
    for (int i = 0; i < FFT_SIZE; i++) {
        input_signal[i] = Complex(rand() % 100, rand() % 100); // Random complex numbers
        local_stream.data = input_signal[i];
        local_stream.last = (i == FFT_SIZE - 1);
        in_stream.write(local_stream);
    }

    // Call FFT hardware function
    fft_8pt(in_stream, out_stream);

    // Read output from hardware
    for (int i = 0; i < FFT_SIZE; i++) {
        local_stream = out_stream.read();
        output_hw[i] = local_stream.data;
    }

    // Software FFT for verification
    for (int k = 0; k < FFT_SIZE; k++) {
        Complex sum = 0;
        for (int n = 0; n < FFT_SIZE; n++) {
            float angle = -2 * PI * k * n / FFT_SIZE;
            Complex Wn(cos(angle), sin(angle)); // Twiddle factor
            sum += input_signal[n] * Wn;
        }
        output_sw[k] = sum;
    }

    // Compare hardware and software outputs
    int error_count = 0;
    for (int i = 0; i < FFT_SIZE; i++) {
        if (std::abs(output_hw[i] - output_sw[i]) > 1e-3) {
            std::cout << "Mismatch at index " << i << ": HW=" << output_hw[i]
                      << ", SW=" << output_sw[i] << std::endl;
            error_count++;
        }
    }

    if (error_count == 0) {
        std::cout << "FFT computation passed!" << std::endl;
    } else {
        std::cout << "FFT computation failed with " << error_count << " errors." << std::endl;
    }

    return 0;
}
