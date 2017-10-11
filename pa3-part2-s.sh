#To create the tun device
openvpn --mktun --dev tun3 
ip link set tun3 up
ip addr add 192.168.7.0/16 dev tun3

#To connect to  the tun_device and get tun descriptor
gcc -o tuns tuns.c
./tuns tun3 10.16.35.111 3445 

#To test whole setup
#gcc -o server1 server1.c
#./server1 192.168.6.41 5000
#xterm -e ./server1 192.168.6.42 5000
#xterm -e ./server1 192.168.6.43 5000









 
