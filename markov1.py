from random import choice
import sys
import os

print '''  
___  ___           _                                           
|  \/  |          | |                                          
| .  . | __ _ _ __| | _______   __   __ _  ___ _ __   ___  ___ 
| |\/| |/ _` | '__| |/ / _ \ \ / /  / _` |/ _ \ '_ \ / _ \/ _ \
| |  | | (_| | |  |   < (_) \ V /  | (_| |  __/ | | |  __/  __/
\_|  |_/\__,_|_|  |_|\_\___/ \_/    \__, |\___|_| |_|\___|\___|
                                     __/ |                     
                                    |___/                      
'''

print "A very simple random markov text gneerator."
print "Here e'd use him as a poet"

def generateModel(text, order):
    model = {}
    for i in range(0, len(text) - order):
        fragment = text[i:i+order]
        next_letter = text[i+order]
        if fragment not in model:
            model[fragment] = {}
        if next_letter not in model[fragment]:
            model[fragment][next_letter] = 1
        else:
            model[fragment][next_letter] += 1
    return model

def getNextCharacter(model, fragment):
    letters = []
    for letter in model[fragment].keys():
        for times in range(0, model[fragment][letter]):
            letters.append(letter)
    return choice(letters)

def generateText(text, order, length):
    model = generateModel(text, order)
    currentFragment = text[0:order]
    output = ""
    for i in range(0, length-order):
        newCharacter = getNextCharacter(model, currentFragment)
        output += newCharacter
        currentFragment = currentFragment[1:] + newCharacter
    
    print " "    
    print "----------------------------- generated speech ------------------------------- \n"
    print output + "\n"
    print "------------------------------------------------------------------------------ \n"

    speakIt(output)

    
def speakIt(corpus):
  speak_cmd = "espeak " + "-a 20 " + "\"" + corpus + "\""
  os.system(speak_cmd)

# text = "some sample text"
raw_data = open('corpus.txt', 'r')
text = raw_data.read()

if __name__ == "__main__":
    generateText(text, int(sys.argv[1]), int(sys.argv[2]))


