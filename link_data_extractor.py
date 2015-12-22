import random
import subprocess
import os
import re


# Open the file
link_file = open("/mnt/sda1/arduino/Poet/links.txt", "r")
links = link_file.read().splitlines()

old_out = "." # a string var to start with
total_links = len(links)

for i in range(5):
  # Using installed elinks cmdline tool to call the links and extract text data from their page
  p = subprocess.Popen(['elinks', '-dump', '-no-references', '-no-numbering', links[i]], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  out, err = p.communicate()
  # Adding all the strings together in a single string.
  out = out + old_out
  old_out = out

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
combined_text_file = open("/mnt/sda1/arduino/Poet/combined_text.txt", "a")
combined_text_file.write(out)

# #Generating random arguments (order and length) for the markov script
# order = random.randrange(0, 20, 2)   #  even int
# length = random.randrange(0, 400, 2) #  even int
# # firing poet
# print "... firing poet"
# poet_cmd = "python markov1.py " + str(order) + " " + str(length)
# print poet_cmd + "\n"
# os.system(poet_cmd)
