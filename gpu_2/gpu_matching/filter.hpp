#include "cl_general.hpp"
#include "../support/matrix/matrix.hpp"
#include "../cpu_matching/cpu_pm.hpp"
#include <vector>

namespace pm {

class gpu_filter_t final : public cl_iclass_t {
public:

    gpu_filter_t(const std::vector<std::string>& patterns) : cl_iclass_t(), patterns_(patterns) 
    {
        build_program({"../gpu_matching/filter.cl"});
    }

    gpu_filter_t(const cl::Device& device , const std::vector<std::string>& patterns) : cl_iclass_t(device), patterns_(patterns)
    {
        build_program({"../gpu_matching/filter.cl"});
    }

    ~gpu_filter_t() = default;

    std::vector<std::vector<std::size_t>> match(std::string& text);

    std::size_t gpu_only_time() const {return gpu_only_time_;}
    std::size_t time() const {return time_;}

private:

    void create_signatures();
    void process_small_patterns(std::vector<std::vector<std::size_t>>& ret, std::string& text);

private:

    static constexpr unsigned min_gpu_process_pattern_size_ = 6;

    std::vector<std::string> patterns_;

    bool signatures_builded_ = false;
    /* class whith contain info for gpu processing */
    struct signature_t {
        static constexpr unsigned rows_ = 256;
        static constexpr unsigned cols_ = 256;

        /* in general signature - matrix 256 * 256, where i, j - first character in pattern A[i][j] - next 4 characters
           indices_ uses to remember id of patterns */
        matrix::matrix_t<unsigned> indices_;
        matrix::matrix_t<cl_float4> data_;
    };

    /* A lot of patterns can have <ij> preffix -> must be a vector of signatures */
    std::vector<signature_t> signature_table_;

    std::size_t time_;
    std::size_t gpu_only_time_ = 0;
};

} /* namespace pm */