from random import choice
import sys
import os

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
    
    # print " "    
    # print "----------------------------- generated speech ------------------------------- \n"
    # print output + "\n"
    # print "------------------------------------------------------------------------------ \n"

    poem_file = open("/mnt/sda1/arduino/Poet/poem.txt", "w")
    poem_file.write(output)
    poem_file.close()

# text = "some sample text"
raw_data = open("/mnt/sda1/arduino/Poet/corpus.txt", 'r')
text = raw_data.read()

if __name__ == "__main__":
    generateText(text, int(sys.argv[1]), int(sys.argv[2]))


