/* 
 *   this kernel compute n preffix functions in n threads:
 *   1 thread for every pattern, local memory not used
 *   ------------------------------------------------------------
 *   patterns (READ ONLY)  - concatenated patterns array
 *   indices  (READ ONLY)  - indices[i] - start of i + 1 pattern
 *   preffix  (WRITE ONLY) - concatenated preffix functions array
 */

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