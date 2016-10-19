#include <stdio.h>


int main()
{
    FILE *fp;

    fp = fopen("sse_prefetch.txt", "r");
    if (!fp) {
        printf("file error\n");
        return -1;
    }

    int d, us;
    double average;
    double sse_prefetch_distance_time[17];
    for (int distance = 0; distance < 17; distance++) {
        for (int i = 0; i < 10; i++) {
            fscanf(fp, "%d %d\n", &d, &us);
            average += (double) us / 10.0;
        }
        sse_prefetch_distance_time[distance] = average;
        average = 0;
    }

    fp = fopen("avx_prefetch.txt", "r");
    if (!fp) {
        printf("file error\n");
        return -1;
    }

    double avx_prefetch_distance_time[17];
    for (int distance = 0; distance < 17; distance++) {
        for (int i = 0; i < 10; i++) {
            fscanf(fp, "%d %d\n", &d, &us);
            average += (double) us / 10.0;
        }
        avx_prefetch_distance_time[distance] = average;
        average = 0;
    }

    fclose(fp);



    FILE *output;

    output = fopen("diff_distance_out.txt", "a");
    if (!output) {
        printf("file error\n");
        return -1;
    }

    for (int i = 0; i < 17; i++) {
        fprintf(output, "%d %s %s %f %f\n", i, "sse_prefetch", "avx_prefetch", sse_prefetch_distance_time[i], avx_prefetch_distance_time[i]);
    }

    fclose(output);
}
