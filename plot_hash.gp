reset
set xlabel 'value'
set ylabel 'hash value'
set style fill solid
set grid
set title 'hash result'
set term png enhanced font 'Verdana,10'
set output 'fig/hash.png'

plot [:1100][:1100] 'dat/generate_1024.dat' with points title 'hash'