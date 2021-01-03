__kernel void bitonic_sort(__global int * data, int stage, int stage_pass, int direction)
{
    int id = get_global_id(0);
    
    int compare_distance = 1 << (stage - stage_pass);

    int left_id = (id % compare_distance) + (id / compare_distance) * 2 * compare_distance;
    int right_id = left_id + compare_distance;
    
    int left_elem = data[left_id];
    int right_elem = data[right_id];
    
    if((id / (1 << stage)) % 2 == 1)
        direction = 1 - direction;

    int greater = right_elem;
    int lesser = left_elem;
    if(left_elem > right_elem) {
        greater = left_elem;
        lesser  = right_elem;
    }
    
    if(direction) {
        data[left_id]  = lesser;
        data[right_id] = greater;
    } else {
        data[left_id]  = greater;
        data[right_id] = lesser;
    }
}
