__kernel void preffix(__global char* patterns, __global ulong* indices, __global ulong* preffix) {
    ulong id           = get_global_id(0);

    ulong right_border = indices[id];
    ulong left_border  = (id > 0) ? indices[id - 1] : 0u;

    //printf("[%d]: %d - %d\n", id, left_border, right_border);

    ulong lenght = right_border - left_border;
    
    for(ulong i = 1; i < lenght; ++i) {
        ulong j = preffix[left_border + i - 1];
        while((j > 0) && (patterns[left_border + i] != patterns[left_border + j])) {
            j = preffix[left_border + j - 1];
        }

        if(patterns[left_border + i] == patterns[left_border + j]) {
            ++j;
        }

        preffix[left_border + i] = j;
    }
    
    
    //for(ulong i = 0; i < lenght; ++i) {
    //    printf("[%d]: %d ", id, preffix[left_border + i]);
    //} 
}

__kernel void kmp(__global char* text, ulong text_size, __global char* patterns, __global ulong* indices, __global ulong* preffix, __global ulong* answer) {
    ulong id = get_global_id(0); // pattern id

    ulong right_border = indices[id];
    ulong left_border  = (id > 0) ? indices[id - 1] : 0u;
    ulong lenght = right_border - left_border;

    //for(ulong g = 0; g < lenght; ++g) {
    //    printf("%d ", preffix[left_border + g]);
    //}

    
    ulong positions_number = 0;
    ulong i = 0; // position of the current character in text
    ulong j = 0; // position of the current character in pattern

    while(i < text_size) {
        //printf("j = %d\n", j);
        //printf("k = %d\n", k);

        if(patterns[left_border + j] == text[i]) {
            ++i; ++j;
        }    
            
        if(j == lenght) {
            answer[id * text_size + positions_number] = i - j + 1;
            ++positions_number;
            j = preffix[left_border + j - 1];
        } else if((i < text_size) && patterns[left_border + j] != text[i]) {
            if (j != 0) {
                j = patterns[left_border + j - 1];
            }     
            else {
                i += 1;
            }     
        }
    }
}