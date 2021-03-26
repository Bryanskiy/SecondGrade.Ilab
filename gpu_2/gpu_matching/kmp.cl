__kernel void kmp(__global char* text, ulong text_size, __global char* pattern, ulong pattern_size, 
                  __global ulong* preffix, ulong preffix_size, __global ulong* answer,
                  __local char* pattern_local, __local ulong* preffix_local) 
{
    /* get info about thread location */
    ulong id = get_global_id(0); // preproc part id
    ulong thread_count = get_global_size(0);

    /* copy pattern and preffix to local memory */
    ulong step = preffix_size / thread_count; /* count of pattern symbols per thread */
    if((preffix_size % thread_count) != 0) {
        step += 1;
    }

    for(ulong i = 0; i < step; ++i) {
        ulong pos = step * id + i;
        if(pos >= pattern_size) {
            break;
        }

        pattern_local[pos] = pattern[pos];
        preffix_local[pos] = preffix[pos];
    }

    barrier(CLK_GLOBAL_MEM_FENCE);

    /* calculate info about text processing part */
    step = text_size / thread_count;
    if((text_size % thread_count) != 0) {
        step += 1;
    }

    ulong left_border  = step * id;
    ulong right_border = min(left_border + step - 1 + pattern_size, text_size);  

    //printf("[%lu]: left_border: %lu | right_border: %lu\n", id, left_border, right_border);

    ulong positions_number = 0;
    ulong i = left_border; // position of the current character in text
    ulong j = 0; // position of the current character in pattern

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