/**
  ******************************************************************************
  * @authors        : Tomas Gonzalez & Brian Morris
  * @file           : process_HR.c
  * @brief          : Source file for process HR
  ******************************************************************************
  */
#include "process_HR.h"

uint16_t max30102_buffer[MAX30102_BUFFER_SIZE];
uint16_t resample_buffer[MAX30102_BUFFER_SIZE / DOWNSAMPLE_FACTOR];
uint16_t buffer_index = 0;
uint32_t bpm = 0;
int peak_count = 0, peak_diff = 0;
uint16_t start, end, elapsed_time_ms;
extern max30102_t max30102;
/**
 * @brief Store red LED sample in buffer and process data when full.
 * @param red_sample Red LED sample value from sensor.
 */
 int buffer_full = 0;
void max30102_put_Buffer(uint32_t red_sample) {


    if (buffer_full) {
        return;  // Evita sobrescribir datos si el buffer está lleno
    }

    if (buffer_index == 0) {
        start = HAL_GetTick();  // Marca el tiempo de inicio
    }

    // Ignorar las primeras muestras no deseadas
    if (buffer_index < MAX30102_UNUSED_DATA) {
        buffer_index++;
        return;
    }

    // Guardar la muestra en el buffer si aún hay espacio
    uint16_t buffer_pos = buffer_index - MAX30102_UNUSED_DATA;
    if (buffer_pos < MAX30102_BUFFER_SIZE) {
        max30102_buffer[buffer_pos] = red_sample;
        buffer_index++;
    }

    // Si se llena el buffer, ejecutar acciones necesarias
    if (buffer_pos == MAX30102_BUFFER_SIZE - 1) {
        buffer_full = 1;
        //max30102_set_a_full(&max30102, 0); // Desactivar IRQ
        //max30102_clear_fifo(&max30102);
        end = HAL_GetTick();
        elapsed_time_ms = end - start;
    }
}

/**
 * @brief Print buffer contents with timestamps.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void print_buffer(uint16_t *buffer, uint16_t size) {
    for (int i = 0; i < size; i++) {
        printf("%d %d\n", buffer[i], elapsed_time_ms * i / size);
    }
}

/**
 * @brief Apply low-pass IIR filter for noise reduction.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void low_pass_filter_IIR(int16_t *buffer, uint16_t size) {
    if (size == 0) return;
    for (uint16_t i = 1; i < size; i++) {
        buffer[i] = (int16_t)(ALPHA * buffer[i] + (1 - ALPHA) * buffer[i - 1]);
    }
}

/**
 * @brief Remove DC component using exponential moving average.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 */
void remove_dynamic_dc_component(uint16_t *buffer, uint16_t size) {
    static uint16_t dc_offset = 0;
    if (size == 0) return;
    for (uint16_t i = 0; i < size; i++) {
        dc_offset = (uint16_t)(ALPHA_DC * buffer[i] + (1 - ALPHA_DC) * dc_offset);
        buffer[i] = (buffer[i] >= dc_offset) ? buffer[i] - dc_offset : 0;
    }
}

/**
 * @brief Check if buffer contains valid data within threshold range.
 * @param buffer Pointer to data buffer.
 * @param size Size of the buffer.
 * @return 1 if data is valid, 0 otherwise.
 */
uint8_t is_data_clear(uint16_t *buffer, uint16_t size) {
    for (int i = 0; i < size; i++) {
        if (buffer[i] < THRESHOLD_LOWER || buffer[i] > THRESHOLD_UPPER) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Downsample input buffer by a given factor.
 * @param input Pointer to input buffer.
 * @param output Pointer to output buffer.
 * @param input_size Size of input buffer.
 * @param output_size Pointer to store output buffer size.
 */
void downsample_buffer(uint16_t *input, uint16_t *output, uint16_t input_size, uint16_t *output_size) {
    uint16_t j = 0;
    for (uint16_t i = 0; i < input_size; i += DOWNSAMPLE_FACTOR) {
        output[j++] = input[i];
    }
    *output_size = j;
}

/**
 * @brief Detect peaks in the signal within a window.
 * @param signal Pointer to signal buffer.
 * @param signal_length Length of signal buffer.
 * @param window_size Size of window to detect peaks.
 */
void find_peaks(uint16_t* signal, uint16_t signal_length, int window_size) {
    int half_window = window_size / 2;
    int first_peak = 0, last_peak = 0;
    peak_count = 0;

    for (int i = half_window; i < signal_length - half_window; i++) {
        int is_peak = 1;
        for (int j = i - half_window; j <= i + half_window; j++) {
            if (signal[i] < signal[j]) {
                is_peak = 0;
                break;
            }
        }
        if (is_peak && signal[i] >= signal[i - 1] && signal[i] >= signal[i + 1] && signal[i - 1] != signal[i]) {
            peak_count++;
            if (first_peak == 0) {
                first_peak = i;
            }
            last_peak = i;
        }
    }
    peak_diff = last_peak - first_peak;
}

/**
 * @brief Process buffer data and calculate BPM.
 */
uint16_t bpm_osc;
void process_buffer(void) {
if(buffer_index==MAX30102_BUFFER_SIZE + MAX30102_UNUSED_DATA){
    if (is_data_clear(max30102_buffer, MAX30102_BUFFER_SIZE)) {
        low_pass_filter_IIR((int16_t*)max30102_buffer, MAX30102_BUFFER_SIZE);
        remove_dynamic_dc_component(max30102_buffer, MAX30102_BUFFER_SIZE);
        uint16_t downsampled_size;
        downsample_buffer(max30102_buffer, resample_buffer, MAX30102_BUFFER_SIZE, &downsampled_size);
        find_peaks(resample_buffer, downsampled_size, PEAK_WINDOW_SIZE);
        elapsed_time_ms = elapsed_time_ms * peak_diff / downsampled_size;
        bpm = peak_count * 60 * 1000 / elapsed_time_ms;       // Revisar este calculo si cambio el tamaño del buffer
        //tambien se puede calcular sabiendo que el periodo del la irq es de =108.24mS (32 samples en c/irq)
        //print_buffer(resample_buffer, downsampled_size);

    }
    buffer_index=0;
    buffer_full=0;
}
}

/**
 * @brief Return BPM
 * @param beats per min
 */
uint16_t get_bpm(void){
	if (bpm<120 && 50<bpm){
		return bpm;
	}else{
	return 0;
	}
}

void reset_bpm(void){
	bpm=0;
}

