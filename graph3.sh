pushd $PARENT > /dev/null

gnuplot <<- EOF

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'cwnd.eps'
set xlabel '{/Helvetica-Oblique Time}'
set ylabel '{/Helvetica-Oblique Cwnd}'
unset key
set title 'CONGESTIONWINDOW'
plot '$CWND' 
EOF

popd > /dev/null
