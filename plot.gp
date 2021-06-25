reset
set xlabel 'value'
set ylabel 'hash value'
set style fill solid
set grid
set title 'hash result'
set term png enhanced font 'Verdana,10'
set output 'hash_1024_modify.png'

plot [:1030][:1030] 'generate.dat' with points title 'hash'