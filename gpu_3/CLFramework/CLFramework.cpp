#include "CLFramework.hpp"
#include "kernels.hpp"

namespace clfm {

/*--------------------------------------------------------------------------------------
                                CLCore
----------------------------------------------------------------------------------------*/

CLCore::Data::Data() {
    cl::Device device = chooseDefaultDevice(); /* can throw exeption */
    cl::Context context(device);
    cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE);
    cl::Program program = buildProgram(context, device); /* can throw exeption */
    /*---------------------------KALB LINE-----------------------------*/
    device_ = std::move(device);
    context_ = std::move(context);
    queue_ = std::move(queue);
    program_ = std::move(program);
}

cl::Program CLCore::Data::buildProgram(cl::Context context, cl::Device device) {
    /* read kernels into string */
    std::string program_string;

    std::ifstream program_sources("../cl_framework/kernels.cl");
    if(!program_sources.good()) {
        program_string = kernels;
    } else {
        auto&& lhs = std::istreambuf_iterator<char>{program_sources};
        std::istreambuf_iterator<char> rhs;
        program_string = std::move(std::string(lhs, rhs));
    }    

    /* build */
    cl::Program::Sources source;
    source = cl::Program::Sources(1, std::make_pair(program_string.c_str(), program_string.length() + 1));
    cl::Program program(context, source);

    try {
        program.build();
    } catch (cl::Error& e) {
        std::cerr << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        throw e;
    } 

    return program;
}

cl::Device CLCore::Data::chooseDefaultDevice() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);


    for(auto&& platform : platforms) {
        std::vector<cl::Device> platform_devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);
        
        for(auto&& device : platform_devices) {
            if(device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>()) {
                return device;
            }
        }
    }

    return cl::Device{};
}

/*--------------------------------------------------------------------------------------
                                CLfVector
----------------------------------------------------------------------------------------*/

void CLfVector::executeBinOp(const std::string& opName, const CLfVector& rhs) {
    std::size_t rhs_size = rhs.size();
    if(size() < rhs_size) {
        data_.resize(rhs_size);
    }

    CLCore::Data cl_data = instance();

    cl::Buffer lhs_buffer(cl_data.context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    cl_data.queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::Buffer rhs_buffer(cl_data.context_, CL_MEM_READ_ONLY, rhs.data_.size() * sizeof(rhs.data_[0]));
    cl_data.queue_.enqueueWriteBuffer(rhs_buffer, CL_TRUE, 0, rhs.data_.size() * sizeof(rhs.data_[0]) , rhs.data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(rhs_size);

    cl::Kernel kernel (cl_data.program_ , opName.c_str());
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , rhs_buffer);

    cl::Event event;
    cl_data.queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    cl_data.queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());
}

CLfVector& CLfVector::operator+=(const CLfVector& rhs) {
    executeBinOp("fVectorAdd", rhs);
    return *this;
}

CLfVector& CLfVector::operator-=(const CLfVector& rhs) {
    executeBinOp("fVectorSub", rhs);
    return *this;
}

CLfVector& CLfVector::operator*=(const CLfVector& rhs) {
    executeBinOp("fVectorMult", rhs);
    return *this;
}

CLfVector& CLfVector::operator*=(float constant) {
    CLCore::Data cl_data = instance();

    cl::Buffer lhs_buffer(cl_data.context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    cl_data.queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(data_.size());

    cl::Kernel kernel (cl_data.program_ , "fVectorConstantMult");
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , constant);

    cl::Event event;
    cl_data.queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    cl_data.queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

float CLfVector::scalarMult(const CLfVector& rhs) const {    
    float ans = 0.0;
    for(std::size_t i = 0, maxi = std::min(data_.size(), rhs.size()); i < maxi; ++i) {
        ans += data_[i] * rhs[i];
    }

    return ans;
}

bool CLfVector::operator<(const CLfVector& rhs) {
    return data_ < rhs.data_;
}

bool CLfVector::operator>(const CLfVector& rhs) {
    return data_ > rhs.data_;
}

bool CLfVector::operator==(const CLfVector& rhs) {
    return data_ == rhs.data_;
}

bool CLfVector::operator!=(const CLfVector& rhs) { 
    return data_ != rhs.data_;
}    
   

const CLfVector operator+(const CLfVector& lhs, const CLfVector& rhs) {
    CLfVector tmp = lhs;
    return tmp += rhs;
}
const CLfVector operator-(const CLfVector& lhs, const CLfVector& rhs) {
    CLfVector tmp = lhs;
    return tmp -= rhs;
}
const CLfVector operator*(const CLfVector& rhs, float constant) {
    CLfVector tmp = rhs;
    return tmp *= constant;
}
const CLfVector operator*(float constant, const CLfVector& rhs) {
    return rhs * constant;
}
const CLfVector operator*(const CLfVector& lhs, const CLfVector& rhs) {
    CLfVector tmp = rhs;
    return tmp *= rhs;
}

/*--------------------------------------------------------------------------------------
                                CLSquareBandetSparseFMatrix 
----------------------------------------------------------------------------------------*/

CLSquareBandetSparseFMatrix::CLSquareBandetSparseFMatrix(const matrix::matrix_t<float>& matrix) {
    std::size_t rows = matrix.get_rows_number();
    std::size_t cols = matrix.get_cols_number();

    if(rows != cols) {
        throw std::runtime_error("CLBandetSparseFFMatrix: invalid size");
    }

    std::size_t size = cols;

    for(std::size_t i = 0; i < size; ++i) {
        CLfVector lower(size);
        CLfVector upper(size);

        for(std::size_t j = 0; j < i + 1; ++j) {
            if(std::abs(matrix[size - 1 - i][size - 1 - i + j]) > accuracy) {
                lower[j] = matrix[size - 1 - i + j][j];
            }
        }

        diagonals_[i] = lower;
        diagonals_[size - 1 - i] = upper;
    }
}

} /* namespace clfm */