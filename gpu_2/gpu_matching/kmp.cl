__kernel void kmp(__global char* text, ulong text_size, __global char* pattern, ulong pattern_size, 
                  __global uint* preffix, ulong preffix_size, __global uint* answer, 
                  __local char* pattern_local, __local uint* preffix_local) 
{
    /* get info about thread location */
    uint id = get_global_id(0); // preproc part id
    uint thread_count = get_global_size(0);

    /* copy to local mem */
    uint local_id = get_local_id(0);
    uint local_thread_count = get_local_size(0);

    uint step = preffix_size / local_thread_count + 1; /* count of pattern symbols per thread */

    for(uint i = 0; i < step; ++i) {
        uint pos = step * local_id + i;
        if(pos >= pattern_size) {
            break;
        }

        pattern_local[pos] = pattern[pos];
        preffix_local[pos] = preffix[pos];
    }

    barrier(CLK_LOCAL_MEM_FENCE);
    
    /* calculate info about text processing part */
    step = text_size / thread_count;
    if((text_size % thread_count) != 0) {
        step += 1;
    }

    uint left_border  = step * id;
    uint right_border = min(left_border + step - 1 + pattern_size, text_size);  

    //printf("[%lu]: left_border: %lu | right_border: %lu\n", id, left_border, right_border);

    /* KMP */
    uint positions_number = 0;
    uint i = left_border; // position of the current character in text
    uint j = 0; // position of the current character in pattern

    while(i < right_border) {
        if(pattern_local[j] == text[i]) {
            ++i; ++j;
        }    
            
        if(j == pattern_size) {
            ++positions_number;
            j = preffix_local[j - 1];
        } else if((i < right_border) && pattern_local[j] != text[i]) {
            if (j > 0) {
                j = preffix_local[j - 1];
            }     
            else {
                i += 1;
            }     
        }
    }


    //printf("[%lu]: %lu\n", id, positions_number);

    answer[id] += positions_number;
}