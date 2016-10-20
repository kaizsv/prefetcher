reset
set style data histogram
set style fill solid
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

if (file eq "diff_distance_out.txt") {
	set ylabel 'time(us)'
	set title 'different distance performance'
	set xlabel 'different distance'

	plot [:][:]'diff_distance_out.txt' using 4:xtic(1) title 'sse_prefetch', \
	'' using 5:xtic(1) title 'avx_prefetch'
}

if (file eq "output.txt") {
	set ylabel 'time ( log2(us) ) '
	set logscale y 2
	set grid
	set title 'different block size'
	set xlabel 'different block size'
	set key left

	plot [:][:]'output.txt' using 7:xtic(1) title 'naive', \
	'' using 8:xtic(1) title 'sse', \
	'' using 9:xtic(1) title 'sse_prefetch', \
	'' using 10:xtic(1) title 'avx', \
	'' using 11:xtic(1) title 'avx_prefetch'
}
