#-------------------------scp into host and deliver a script
# you need to provide ssh password free acess
# learn it from here: https://yorkspace.wordpress.com/2009/04/08/using-public-keys-with-dropbear-ssh-client/


echo " "

scp dumper.sh root@192.168.8.1: -i ~/.ssh/id_rsa # 192.168.8.1 beingthe default AP ip of wrtnode
echo " "
echo "dumped the scraper sript in router"
echo " "

#-------------------------ssh into host and  run the script

sleep 2

ssh root@192.168.8.1 -i ~/.ssh/id_rsa 'chmod u+x dumper.sh'
echo "made the dumperScript executable"

sleep 2

ssh root@192.168.8.1 -i ~/.ssh/id_rsa 'chmod 0600 dumper.sh'
echo "executable script confirmed"

#---------------------------start the main sraping and post delivery script

sleep 2
# $1 is the variable to hold the on of seconds you wanna run the dumper scriopt <you pass itas an argument to the command>
# $2 is the variable to hold the on ip of target home computer
# $3 is the variable to hold the on ip of Yun

 
ssh root@192.168.8.1 -i ~/.ssh/id_rsa sh dumper.sh $1 $2 $3


echo " "