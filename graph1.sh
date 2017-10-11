pushd $PARENT > /dev/null

tcpdump -r $PCAP0 | grep -v 'seq' | cut -d ' ' -f 1,9 | cut -d ':' -f 3 | cut -d ',' -f 1 > graph1data1.txt
tcpdump -r $PCAP0 | grep -i 'ack 1,' | cut -d ' ' -f 1,9 | cut -d ':' -f 3 | cut -d ',' -f 1 > graph1data2.txt

gnuplot <<- EOF
set term postscript eps color
set output 'seqack.eps'
iset xlabel 'Time'
set ylabel 'SeqAck.no'
plot 'graph1data1.txt' t'ack' with lines, 'graph1data2.txt' t'seq' with lines
EOF

popd > /dev/null
 
