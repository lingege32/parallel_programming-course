__kernel void histogram(__global unsigned char* input_data,
						__global unsigned int* ref_histogram_results,
						unsigned int input_size)
{
	const size_t index = get_global_id(0);
	if(index<input_size){
		int rgbIndex=index%3;
		atomic_inc(ref_histogram_results + (input_data[index] + rgbIndex * 256));
	}
}
