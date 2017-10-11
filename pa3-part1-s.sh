#!/bin/bash

echo "1" > /proc/sys/net/ipv4/ip_forward

modprobe ip_tables

modprobe ip_conntrack

modprobe ip_conntrack_irc

modprobe ip_conntrack_ftp

iptables -t nat -F

iptables -t nat -A PREROUTING -p tcp  -s 10.16.35.8 -d 10.16.35.111 --dport 4141 -j DNAT --to-destination 192.168.6.41:5000

iptables -t nat -A PREROUTING -p tcp  -s 10.16.35.8 -d 10.16.35.111 --dport 4242 -j DNAT --to-destination 192.168.6.42:5000

iptables -t nat -A PREROUTING -p tcp  -s 10.16.35.8 -d 10.16.35.111 --dport 4343 -j DNAT --to-destination 192.168.6.43:5000


