
void get_words(__global char* pkt_buffer, 
                 const size_t buffer_size, 
                 const size_t pos, 
                 float2 *word0, 
                 float2 *word1, 
                 float4 *tag0, 
                 float4 *tag1)
{
    const size_t border = min(7lu, buffer_size - pos);

    (*word0).x = (border >= 1) ? pkt_buffer[pos] : 0.0;
    (*word0).y = (*word1).x = (border >= 2) ? pkt_buffer[pos + 1] : 0.0;
    (*word1).y = (*tag0).s0 = (border >= 3) ? pkt_buffer[pos + 2] : 0.0;
    (*tag0).s1 = (*tag1).s0 = (border >= 4) ? pkt_buffer[pos + 3] : 0.0;
    (*tag0).s2 = (*tag1).s1 = (border >= 5) ? pkt_buffer[pos + 4] : 0.0;
    (*tag0).s3 = (*tag1).s2 = (border >= 6) ? pkt_buffer[pos + 5] : 0.0;
    (*tag1).s3 = (border >= 1) ? pkt_buffer[pos + 6] : 0.0;
}




__kernel void signature_match(__global char*    pkt_buffer, 
                              __global float2*  res_buffer, 
                                const uint      buffer_size, 
                              __read_only       image2d_t g_table)
{
    const size_t id = get_global_id(0);

    const size_t fst = id * 2;
    const size_t scd = fst + 1;

    float2 word0 = (float2)(0.0, 0.0);
    float2 word1 = (float2)(0.0, 0.0);
    float4 tag0 =  (float4)(0.0, 0.0, 0.0, 0.0);
    float4 tag1 =  (float4)(0.0, 0.0, 0.0, 0.0);

    get_words(pkt_buffer, buffer_size, fst, &word0, &word1, &tag0, &tag1);

    const float4 h0 = read_imagef(g_table, convert_int2(convert_uchar2(convert_char2(word0.yx))));
    const float4 h1 = read_imagef(g_table, convert_int2(convert_uchar2(convert_char2(word1.yx))));

    float all_match0 = all(h0 == tag0);
    float all_match1 = all(h1 == tag1);

    res_buffer[fst] = word0 * all_match0;
    res_buffer[scd] = word1 * all_match1;
}