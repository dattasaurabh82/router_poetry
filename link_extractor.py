import os
import subprocess

print '''
 _     _       _      _   _             _            
| |   (_)     | |    | | | |           | |           
| |    _ _ __ | | __ | |_| |_   _ _ __ | |_ ___ _ __ 
| |   | | '_ \| |/ / |  _  | | | | '_ \| __/ _ \ '__|
| |___| | | | |   <  | | | | |_| | | | | ||  __/ |   
|_____/_|_| |_|_|\_\ |_| |_/\__,_|_| |_|\__\___|_|   
                                                     

'''
print '''
----------------------------------------------------------------------------------------------------------------- 
|  I'm first trying to find the links of the servers and few websites that I can access from your browsing data |
|  that was captured. I'll take some time as my processor is very slow. I'm going to run late don't worry just  |
|  hold tight.                                                                                                  |
-----------------------------------------------------------------------------------------------------------------
'''

# You can extend this list as you go ..
domain_array = ['.com', '.org', '.net', '.int', '.gov', '.mil', '.uk']
link_list = []

pcap_data_txt = open('data.txt', 'r')
raw_pcap_data = pcap_data_txt.read().replace('\n', '')
# print (raw_pcap_data) # print the actual content

for index in range (len(domain_array)):
  idx = 0
  domain_name = domain_array[index]
  each_domain_count = raw_pcap_data.count(domain_name)
  index_of_domain  = [i for i in range(len(raw_pcap_data)) if raw_pcap_data.startswith(domain_name, i)]
  # --------- regex (not so efficient formula - you can make it better with your knwledge)

  # print domain_name, " : ", "total >", each_domain_count, " Indices > ", index_of_domain # counting domains in total #------------------------un-comment for Debugging

  domain_len = len(domain_name) # e.g. >>> print len('.com') >>> 4 >>> print len('.cn') >>> 3 ... etc. you get the idea
  # will be used later to help recreating the full link string

  max_cap_prob_len = 40 # an approx no. to play with for grabbing parts of the string with the domain endings for future finding of 'www.'
  for idx in range (len(index_of_domain)):
    # --------- create a n approximate length of a string where we find the domains backwards to include 'www.' for reconstruction of links.
    probable_links = raw_pcap_data[index_of_domain[idx] - max_cap_prob_len: index_of_domain[idx] + domain_len]
    www_header_index = probable_links.find('www.')
    if www_header_index != -1: # exception handling as for, if it can not find it, it throws a -1 and we use this -
      # -to check and eliminate concataneted links with out 'www.''
      # --------- Recreating first set of unfiltered links such as www.xxx.com or www.xxx.org etc.
      first_recon_links = probable_links[www_header_index: len(probable_links)]
      # --------- finding and removing '..' from links and other empty spaces.
      if first_recon_links.find('..') == -1: # find method always return a -1 if it cannot find the attribute
        # print first_recon_links
        # --------- Putting data in a list
        link_list.append(first_recon_links)

link_list = list(set(link_list)) # for removing duplicates
print link_list
#------- --------------------- --------To do Now ---- -----------------------#
# Run a loop through the list and for each link open it in libreOffice or Lynx and append the result in a txt file.  
# Thus creating all the content of the html from all the links in a single txt file.

# This will be a corpus. 
# 
# #--------------------------------------------------------------------------#
#                            ||
#                            \/
for k in range(len(link_list)):
  #print link_list[k]
  # writing the links to a text file
  link_file = open("links.txt", "a")
  link_file.write(link_list[k] + '\n')
  link_file.close()

print "All the extracted links have been written to \"links.txt\" \n"












