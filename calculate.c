#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int is_block_size = atoi(argv[1]);

    if (is_block_size) {
        FILE *fp;

        fp = fopen("naive.txt", "r");
        if (!fp) {
            printf("file error\n");
            return -1;
        }

        double us, average;
        double naive_diff_block_time[4];
        for (int block = 0; block < 4; block++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%lf\n", &us);
                average += us / 10.0;
            }
            naive_diff_block_time[block] = average;
            average = 0;
        }

        fp = fopen("sse.txt", "r");
        if (!fp) {
            printf("file error\n");
            return -1;
        }

        double sse_diff_block_time[4];
        for (int block = 0; block < 4; block++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%lf\n", &us);
                average += us / 10.0;
            }
            sse_diff_block_time[block] = average;
            average = 0;
        }

        fp = fopen("sse_prefetch.txt", "r");
        if (!fp) {
            printf("file error\n");
            return -1;
        }

        int d;
        double sse_prefetch_block_size[4];
        for (int block = 0; block < 4; block++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%d %lf\n", &d, &us);
                average += us / 10.0;
            }
            sse_prefetch_block_size[block] = average;
            average = 0;
        }

        fp = fopen("avx.txt", "r");
        if (!fp) {
            printf("file error\n");
            return -1;
        }

        double avx_diff_block_size[4];
        for (int block = 0; block < 4; block++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%lf\n", &us);
                average += us / 10.0;
            }
            avx_diff_block_size[block] = average;
            average = 0;
        }

        fp = fopen("avx_prefetch.txt", "r");
        if (!fp) {
            printf("fiel error\n");
            return -1;
        }

        double avx_prefetch_block_size[4];
        int size[4];
        for (int block = 0; block < 4; block++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%d %lf\n", &d, &us);
                average += us / 10.0;
                size[block] = d;
            }
            avx_prefetch_block_size[block] = average;
            average = 0;
        }

        fclose(fp);

        FILE *output;
        output = fopen("output.txt", "a");
        for (int block = 0; block < 4; block++) {
            fprintf(output, "%d %s %s %s %s %s %lf %lf %lf %lf %lf\n",
                    size[block],
                    "naive", "sse", "sse_prefetch", "avx", "avx_prefetch",
                    naive_diff_block_time[block], sse_diff_block_time[block],
                    sse_prefetch_block_size[block], avx_diff_block_size[block],
                    avx_prefetch_block_size[block]);
        }

        fclose(output);


    } else {

        FILE *fp;

        fp = fopen("sse_prefetch.txt", "r");
        if (!fp) {
            printf("file error\n");
            return -1;
        }

        int d;
        double average, us;
        double sse_prefetch_distance_time[17];
        for (int distance = 0; distance < 17; distance++) {
            for (int i = 0; i < 10; i++) {
                fscanf(fp, "%d %lf\n", &d, &us);
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
                fscanf(fp, "%d %lf\n", &d, &us);
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
            fprintf(output, "%d %s %s %lf %lf\n", i, "sse_prefetch", "avx_prefetch", sse_prefetch_distance_time[i], avx_prefetch_distance_time[i]);
        }

        fclose(output);
    }
}
