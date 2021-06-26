reset
set xlabel 'iteration'
set ylabel 'time (sec)'
set style fill solid
set grid
set title 'Execution time for hashing 10^9 time'
set term png enhanced font 'Verdana,10'
set output 'fig/speed.png'

plot [1:31][1.8:1.9] 'dat/speed.dat' using 1 with linespoints title '0x9E3779BA', \
'' using 2 with linespoints title '0x61C88647'