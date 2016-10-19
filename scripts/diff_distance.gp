reset
set ylabel 'time(us)'
set xlabel 'different distance'
set style data histogram
set style fill solid
set title 'different distance performance'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:]'diff_distance_out.txt' using 4:xtic(1) title 'sse_prefetch', \
'' using 5:xtic(1) title 'avx_prefetch'
