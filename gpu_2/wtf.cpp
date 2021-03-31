#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <vector>
#include <iostream>
#include <string>

int main() {

	cl::Program program;
	cl::Device device;

try {

	std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    std::vector<cl::Device> platform_devices;
    platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);

    device = platform_devices[0];
    cl::Context context(device);
    cl::CommandQueue queue(context, device);
	std::string program_string = 
	"__kernel void k(__global int * input, __global int * output) { "
	"     uint id = get_global_id(0); "		
	"	  output[id] = input[id] + 1; "
	"}";

    cl::Program::Sources source(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    program = cl::Program(context, source);
    program.build();

    std::vector<int> input(10, 0);
    std::vector<int> output(10, 0);
    cl::Buffer input_buffer(context, CL_MEM_READ_ONLY, input.size() * sizeof(input[0]));
	cl::Buffer output_buffer(context, CL_MEM_WRITE_ONLY, output.size() * sizeof(output[0]));

	queue.enqueueWriteBuffer(input_buffer, CL_TRUE, 0, input.size() * sizeof(input[0]), input.data());
	queue.enqueueWriteBuffer(output_buffer, CL_TRUE, 0, output.size() * sizeof(output[0]), output.data());

	cl::NDRange offset(0);
    cl::NDRange global_size(10);
    cl::NDRange local_size(1);

    cl::Kernel kernel(program, "k");
    kernel.setArg(0, input_buffer);
    kernel.setArg(1, output_buffer);

    queue.enqueueNDRangeKernel(kernel, offset, global_size, local_size, NULL, NULL);
    //queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output.size() * sizeof(output[0]), output.data());
    //cl::copy(output_buffer, output.begin(), output.end());


    for(std::size_t i = 0; i < 10; ++i) {
     	std::cout << output[i] << " ";
    }

}

catch(cl::Error& er) {	
	std::cout << er.what() << std::endl;
}	
}