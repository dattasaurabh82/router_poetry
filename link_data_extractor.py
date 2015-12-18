import random
import subprocess
import os
import re

print '''  
 _____                              _____                           _             
/  __ \                            |  __ \                         | |            
| /  \/ ___  _ __ _ __  _   _ ___  | |  \/ ___ _ __   ___ _ __ __ _| |_ ___  _ __ 
| |    / _ \| '__| '_ \| | | / __| | | __ / _ \ '_ \ / _ \ '__/ _` | __/ _ \| '__|
| \__/\ (_) | |  | |_) | |_| \__ \ | |_\ \  __/ | | |  __/ | | (_| | || (_) | |   
 \____/\___/|_|  | .__/ \__,_|___/  \____/\___|_| |_|\___|_|  \__,_|\__\___/|_|   
                 | |                                                              
                 |_|  

'''

print "... starting data capture form links"


# Open the file
link_file = open("links.txt", "r")
links = link_file.read().splitlines()

old_out = "." # a string var to start with
total_links = len(links)

for i in range(total_links):
  # Using installed elinks cmdline tool to call the links and extract text data from their page
  p = subprocess.Popen(['elinks', '-dump', '-no-references', '-no-numbering', links[i]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  out, err = p.communicate()
  # Adding all the strings together in a single string. 
  out = out + old_out
  old_out = out
  # to keep track of response scraping from the website call and text grabbing  
  print i, " txt grabbed and added to the previous texts \n \n" 

# Reducing the text out of few things
replacement_char = ["_", "|", ">", "<", ";", "{", "}", "(", ")", "[", "]", "-", "\"", "\\", "."] # for checking and adding more stuff for either remobval or replacements

for i in range (0, len(replacement_char)):
  if replacement_char[i] is "-":
    out = out.replace(replacement_char[i], "Dash")
  elif replacement_char[i] is "\"":
    out = out.replace(replacement_char[i], "QUOTE")
  elif replacement_char[i] is "-":
    out = out.replace(replacement_char[i], "Dash")
  elif replacement_char[i] is "\ ":
    out = out.replace(replacement_char[i], "\\")
  elif replacement_char[i] is " \ ":
    out = out.replace(replacement_char[i], "\\")
  else:
    out = out.replace(replacement_char[i], " ")

# Saving thencontent in a txt file
combined_text_file = open("combined_text.txt", "a")
combined_text_file.write(out)
# combined_text_file.close

try:
  # Removing white spaces by running a terminal command
  os.system("sed -e 's/[\t ]//g;/^$/d' combined_text.txt > combined.txt")
  print "... Removed white spaces"
  try:
    # combining multiple lines into one
    os.system("echo $(cat combined.txt) > corpus.txt")
    print "... Combined multiple lines"

    # Show the content may be .. 
  except (RuntimeError, TypeError, NameError, IOError, ValueError):
    print '''
    Something happeded .. couldn't remove stuff. may be file is not there or
    some other error
    '''
except (RuntimeError, TypeError, NameError, IOError, ValueError):
  print '''
  Something happeded .. couldn't remove stuff. may be file is not there or
  some other error
  '''
os.remove('combined.txt')
os.remove('combined_text.txt')

print "... Corpus generated :) \n"

#Generating random arguments (order and length) for the markov script
order = random.randrange(0, 20, 2)   #  even int
length = random.randrange(0, 400, 2) #  even int
# firing poet
print "... firing poet"
poet_cmd = "python markov1.py " + str(order) + " " + str(length)
print poet_cmd + "\n"
os.system(poet_cmd)
