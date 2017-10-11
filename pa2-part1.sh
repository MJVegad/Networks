export PARENT=$1

pushd $PARENT > /dev/null

export PCAP0=$(ls *0-0.pcap)
export PCAP1=$(ls *1-0.pcap)
export CWND=$(ls *.cwnd)
export TRACE=$(ls *.tr)

popd > /dev/null

echo "started generating graphs..."
echo ""
bash graph1.sh
echo "Generated graph for sequence no./ Ack no. vs time."
echo "Files used:: graph1data1.txt, graph1data2.txt." 
echo ""
bash graph2.sh
echo "Generated graph for Avg. throughput per window of second."
echo "File used:: tputgraph.txt."
echo ""
bash graph3.sh
echo "Generated graph for Congestion window vs time."
echo "File used:: tcp-example.cwnd."
echo ""
echo "Processing, please wait................."
echo ""
bash graph4.sh
echo "Generated graph for Queue occupancy vs time."
echo "File used:: queuegraph.txt."
echo ""
bash graph5.sh
echo "Generated graph for Waiting time vs departure time."
echo "File used:: qdelaysgraph.txt."
echo ""
echo "All the graphs generated successfully..!!"
echo ""

