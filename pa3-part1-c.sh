#!/bin/bash

echo "1" > /proc/sys/net/ipv4/ip_forward

modprobe ip_tables

modprobe ip_conntrack

modprobe ip_conntrack_irc

modprobe ip_conntrack_ftp

iptables -t nat -F

iptables -t nat -A OUTPUT -p tcp -s 192.168.7.41 -d 192.168.6.41 --dport 5000 -j DNAT --to-destination 10.16.35.111:14000
iptables -t nat -A POSTROUTING -p tcp -s 192.168.7.41 -d 10.16.35.111 --dport 14000 -j MASQUERADE

iptables -t nat -A OUTPUT -p tcp -s 192.168.7.42 -d 192.168.6.42 --dport 5000 -j DNAT --to-destination 10.16.35.111:14010
iptables -t nat -A POSTROUTING -p tcp -s 192.168.7.42 -d 10.16.35.111 --dport 14010 -j MASQUERADE

iptables -t nat -A OUTPUT -p tcp -s 192.168.7.43 -d 192.168.6.43 --dport 5000 -j DNAT --to-destination 10.16.35.111:14020
iptables -t nat -A POSTROUTING -p tcp -s 192.158.7.43 -d 10.16.35.111 --dport 14020 -j MASQUERADE
