#To create the tun device
openvpn --mktun --dev tun3 
ip link set tun3 up
ip addr add 192.168.6.0/16 dev tun3


#To connect to  the tun_device and get tun descriptor
gcc -o tuns tuns.c
./tunc tun3 10.16.35.111 3445 
