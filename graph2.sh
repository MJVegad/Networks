pushd $PARENT > /dev/null

tcpdump -r $PCAP1 | sed 's/05:30://g' | grep -i '536$' | awk '{if($1~/^01/) m++; else if($1~/^02/) n++; else if($1~/^03/) o++; else if($1~/^04/) p++; else if($1~/^05/) q++; else if($1~/^06/) r++; else if($1~/^07/) s++;else if($1~/^08/) t++; else if($1~/^09/) u++;  } END{printf("1 %f Mbits/sec\n2 %f Mbits/sec\n3 %f Mbits/sec\n4 %f Mbits/sec\n5 %f Mbits/sec\n6 %f Mbits/sec\n7 %f Mbits/sec\n8 %f Mbits/sec\n9 %f Mbits/sec\n",m*536*8/1000000,n*536*8/1000000,o*536*8/1000000,p*536*8/1000000,q*536*8/1000000,r*536*8/1000000,s*536*8/1000000,t*536*8/1000000,u*536*8/1000000)}' > tputgraph.txt 

gnuplot <<- EOF

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'Avgthroughput.eps'
set xlabel '{/Helvetica-Oblique Time}'
set ylabel '{/Helvetica-Oblique Mbits/sec}'
set grid
set title 'THROUGHPUT'
unset key
plot 'tputgraph.txt' using 1:2 t'throughput' with lines
EOF

popd > /dev/null
 
