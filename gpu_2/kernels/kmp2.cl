/* 
 *   this kernel find substiongs in n threads:
 *   
 *   ------------------------------------------------------------
 */

__kernel void kmp2(__global char* text, ulong text_size, __global char* patterns, __global ulong* indices, __global ulong* preffix, __global ulong* answer) {
    ulong id = get_global_id(0); // pattern id

    ulong right_border = indices[id];
    ulong left_border  = (id > 0) ? indices[id - 1] : 0u;
    ulong lenght = right_border - left_border;

    //printf("[%d]: preffix_lenggth = %d\n", id, lenght);

    //for(ulong g = 0; g < lenght; ++g) {
    //    printf("[%d]: %d ", id, preffix[left_border + g]);
    //}

    ulong positions_number = 0;
    ulong i = 0; // position of the current character in text
    ulong j = 0; // position of the current character in pattern

    while(i < text_size) {
        //printf("[%d]: i = %lu\n", id, i);
        //printf("[%d]: j = %lu\n", id, j);

        if(patterns[left_border + j] == text[i]) {
            ++i; ++j;
        }    
            
        if(j == lenght) {
            answer[id * text_size + positions_number] = i - j + 1;
            ++positions_number;
            j = preffix[left_border + j - 1];
        } else if((i < text_size) && patterns[left_border + j] != text[i]) {
            if (j > 0) {
                j = preffix[left_border + j - 1];
            }     
            else {
                i += 1;
            }     
        }
    }
}