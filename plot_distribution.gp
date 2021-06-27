reset
set xlabel 'bucket num'
set ylabel 'num of key in bucket'
set style fill solid
set grid
set title 'distribution'
set term png enhanced font 'Verdana,10'
set output 'fig/distribution.png'

plot [0:1024][0:5] 'dat/distribution_1024.dat' using 1 with histogram title 'key count'