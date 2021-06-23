#pragma once 

#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include "../support/matrix/matrix.hpp"

#include <stdexcept>
#include <iostream>
#include <map>
#include <fstream>

namespace clfm {

/*--------------------------------------------------------------------------------------
                                CLCore
----------------------------------------------------------------------------------------*/

constexpr float accuracy = 1e-5;

class CLCore {
protected:

    struct Data {
        Data();
        
        cl::Device device_;
        cl::Context context_;
        cl::CommandQueue queue_;
        cl::Program program_;

    private:
        cl::Device chooseDefaultDevice();
        cl::Program buildProgram(cl::Context context, cl::Device device);
    };

    static Data& instance() {
        static Data core;
        return core;
    }

    CLCore() = default;
    CLCore(const CLCore& rhs) = default;
    CLCore& operator=(const CLCore& rhs) = default;

private:
    CLCore(CLCore&& rhs) = delete;
    CLCore& operator=(CLCore&& rhs) = delete;
}; 

/*--------------------------------------------------------------------------------------
                                CLfVector
----------------------------------------------------------------------------------------*/

class CLfVector final : public CLCore {
public:    
    explicit CLfVector(std::size_t size) : data_(size) {}

    template<typename InputIt>
    CLfVector(InputIt start, InputIt end) : data_{start, end} {}

    CLfVector() = default;
    CLfVector(const CLfVector& rhs) : data_{rhs.data_} {}
    CLfVector(const std::initializer_list<float>& list) : data_{list} {}

    std::vector<float> unpack() const {return data_;}
    std::size_t size() const { return data_.size(); }
    void resize(std::size_t size) {data_.resize(size); }

    CLfVector& operator=(const CLfVector& rhs) = default;
    float& operator[](std::size_t idx) {return data_[idx];}
    const float& operator[](std::size_t idx) const {return const_cast<CLfVector&>(*this)[idx];}

    CLfVector& operator+=(const CLfVector& rhs);
    CLfVector& operator-=(const CLfVector& rhs);
    CLfVector& operator*=(const CLfVector& rhs);
    CLfVector& operator*=(float constant);

    bool operator<(const CLfVector& lhs);
    bool operator>(const CLfVector& lhs);
    bool operator==(const CLfVector& lhs);
    bool operator!=(const CLfVector& lhs);

    float scalarMult(const CLfVector& rhs) const;

private:

    /* opName - string instead of opcode, because kernel has function name argument */
    void executeBinOp(const std::string& opName, const CLfVector& rhs);

private:
    std::vector<float> data_;
};

const CLfVector operator+(const CLfVector& lhs, const CLfVector& rhs);
const CLfVector operator-(const CLfVector& lhs, const CLfVector& rhs);
const CLfVector operator*(const CLfVector& rhs, float constant);
const CLfVector operator*(float constant, const CLfVector& rhs);
const CLfVector operator*(const CLfVector& lhs, const CLfVector& rhs);

/*--------------------------------------------------------------------------------------
                                CLSquareBandetSparseFMatrix 
----------------------------------------------------------------------------------------*/

class CLSquareBandetSparseFMatrix final : public CLCore {
public: 
    CLSquareBandetSparseFMatrix(const matrix::matrix_t<float>& matrix);

    CLfVector operator*(const CLfVector& rhs);
private:
    std::map<std::size_t, CLfVector> diagonals_;
};

} /* namespace clfm */
