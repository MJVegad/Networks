#!/bin/bash

echo "Generate seqence numbers of node 0 in n0seq.dat file"
tshark -T fields -n -r $1/foo.pcap -Y "ip.src==137.132.155.197" -E separator=/s -e frame.time_relative -e tcp.seq > $1/n0seq.dat

echo "Generate ack numbers of node 0 in n0ack.dat file"
tshark -T fields -n -r $1/foo.pcap -Y "ip.src==10.9.162.106" -E separator=/s -e frame.time_relative -e tcp.ack > $1/n0ack.dat

echo "Plotting graph of seq and ack vs time in TCP-time-sequence-graph.png"
gnuplot << EOF
set terminal png
set output '$1/TCP-time-sequence-grapn.png'
set ylabel "Seq/Ack" textcolor lt 1
set xlabel "Time" textcolor lt 2
set format y "%.0f"
plot [0:100] "$1/n0seq.dat" t "Seqence nos" w d, "$1/n0ack.dat" t "Acknowledgement nos" w d
EOF

echo "Generate the total frames sent in framesent.dat"
tshark -T fields -n -r $1/foo.pcap -Y "ip.dst==10.9.162.106" -E separator=/s -e frame.time_relative -e frame.len > $1/framesent.dat

echo "Generate the total bytes sent in tp.dat"
awk '
BEGIN { q=0;t=0;}
{ q = q + $2;
if($1 >= t)
{
	print t " " q;
	t = t + 1;
	q=0;
}
}
END{}
' $1/framesent.dat > $1/tp.dat

echo "Plotting graph of Average Throughput vs time in Avg-throughput.png"
gnuplot << EOF

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'Avgthroughput.eps'
set ylabel "Avg-throughput (Mbits/sec)"
set xlabel "Time"
plot "$1/tp.dat" u 1:((\$2*8)/(1024*1024)) t "Average-Throughput"


EOF
