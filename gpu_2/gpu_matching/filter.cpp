#include "filter.hpp"

namespace pm {

std::vector<std::vector<std::size_t>> gpu_filter_t::match(std::string& text) {
    Timer_t timer;

    std::vector<std::vector<std::size_t>> ret(patterns_.size());

    if(!signatures_builded_) {
        create_signatures();
        signatures_builded_ = true;
    }    

    process_small_patterns(ret, text);

    cl::Buffer text_buffer(context_, CL_MEM_READ_ONLY, text.size() * sizeof(text[0]));
    queue_.enqueueWriteBuffer(text_buffer, CL_TRUE, 0, text.size() * sizeof(text[0]), text.data());

    std::size_t table_depth = signature_table_.size();

    std::vector<cl::Event> events(table_depth);
    cl::NDRange offset(0);
    cl::NDRange global_size(text.size() / 2 + text.size() % 2);

    /* create images */
    std::vector<cl::Image2D> images;
    images.reserve(table_depth);
    cl::ImageFormat format(CL_RGBA, CL_FLOAT);
    for(std::size_t i = 0; i < table_depth; ++i) {
        images.emplace_back(context_, CL_MEM_READ_ONLY, format, signature_t::rows_, signature_t::rows_);
    }

    /* create answer buffers */
    std::vector<cl::Buffer> answer_buffers;
    answer_buffers.reserve(table_depth);
    for(std::size_t i = 0; i < table_depth; ++i) {
        answer_buffers.emplace_back(context_, CL_MEM_WRITE_ONLY, text.size() * sizeof(cl_float2));
    }
 
    /* send kernels to gpu */
    for(std::size_t i = 0; i < table_depth; ++i) {

        cl::size_t<3> origin, region;
        origin[0] = 0; origin[1] = 0; origin[2] = 0; 
        region[0] = signature_t::rows_; region[1] = signature_t::cols_; region[2] = 1;

        queue_.enqueueWriteImage(images[i], CL_TRUE, origin, region, 0, 0, signature_table_[i].data_.data());

        cl::Kernel kernel(program_, "signature_match");
        kernel.setArg(0, text_buffer);
        kernel.setArg(1, answer_buffers[i]);
        kernel.setArg(2, static_cast<cl_uint>(text.size()));
        kernel.setArg(3, images[i]);

        queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &events.at(i));
    }

    std::vector<cl_float2> answer_vector(text.size());

    /* send kernels to gpu */
    for(std::size_t step = 0; step < table_depth; ++step) {

        events[step].wait();
        std::size_t start = events[step].getProfilingInfo<CL_PROFILING_COMMAND_START>();
        std::size_t end = events[step].getProfilingInfo<CL_PROFILING_COMMAND_END>();
        gpu_only_time_ += (end - start) / 1000;

        queue_.enqueueReadBuffer(answer_buffers[step], CL_TRUE, 0, answer_vector.size() * sizeof(answer_vector[0]), answer_vector.data());
        for (size_t i = 0; i < text.size(); ++i) {
            cl_float2 cur_item = answer_vector[i];

            u_char lhs = static_cast<u_char>(static_cast<char>(cur_item.s[0]));
            u_char rhs = static_cast<u_char>(static_cast<char>(cur_item.s[1]));

            if(lhs != 0 && rhs != 0) {

                std::size_t pattern_idx = signature_table_[step].indices_[lhs][rhs];

                if (text.find(patterns_[pattern_idx], i) != std::string::npos) {
                    ret[pattern_idx].push_back(i);
                }

            }
        }
    }
    time_ = timer.get_time().count();
    return ret;
}

void gpu_filter_t::create_signatures() {
std::vector<std::vector<std::vector<std::size_t>>> patterns_table(signature_t::rows_);
    for(auto&& col : patterns_table) {
        col.resize(signature_t::cols_);
    }

    std::size_t depth = 0;
    for(std::size_t i = 0, max = patterns_.size(); i < max; ++i) {
        if(patterns_[i].size() >= min_gpu_process_pattern_size_) {
            unsigned char fst = patterns_[i][0];
            unsigned char scd = patterns_[i][1];

            patterns_table[fst][scd].push_back(i);
            depth = std::max(depth, patterns_table[fst][scd].size());
        } 
    }

    signature_table_.reserve(depth);
    for(std::size_t stage = 0; stage < depth; ++stage) {
        signature_t signature;

        signature.indices_.resize(signature_t::rows_, signature_t::cols_);
        signature.data_.resize(signature_t::rows_, signature_t::cols_);

        for(std::size_t i = 0; i < signature_t::rows_; ++i) {
            for(std::size_t j = 0; j < signature_t::cols_; ++j) {

                if(patterns_table[i][j].size() <= stage) {
                    continue;
                }

                if(patterns_table[i][j].empty()) {
                    continue;
                }

                std::size_t pattern_idx = patterns_table[i][j][stage];
                if(patterns_[pattern_idx].size() < min_gpu_process_pattern_size_) {
                    continue;
                }

                signature.indices_[i][j] = pattern_idx;

                signature.data_[i][j].s[0] = patterns_[pattern_idx][2];
                signature.data_[i][j].s[1] = patterns_[pattern_idx][3];
                signature.data_[i][j].s[2] = patterns_[pattern_idx][4];
                signature.data_[i][j].s[3] = patterns_[pattern_idx][5];
            }
        }

        signature_table_.push_back(signature);
    }
}

void gpu_filter_t::process_small_patterns(std::vector<std::vector<std::size_t>>& ret, std::string& text) {
    for(std::size_t i = 0, maxi = patterns_.size(); i < maxi; ++i) {
        if(patterns_[i].size() < min_gpu_process_pattern_size_) {
            ret[i] = pm::match(text, patterns_[i]);
        }
    }
}

} /* namspace pm */