#!/usr/bin/python3
import re, fnmatch, os

for file in os.listdir("static/"):
    if (file.endswith(".html")): print(file)

# Extract HTML file body 
with open("static/header.html","r") as f:
    htmlString = f.read()
    f.close()

bodyIndex = [htmlString.find("<body>"),htmlString.find("</body>")]
htmlBody = htmlString[bodyIndex[0]:bodyIndex[1]]

# Find HTML body location in header
with open("include/TestStrings.h","r") as f:
    headerString = f.read()
    f.close()

bodyIndex = headerString.find("String bodyString = {R\"=====(\n)=====\"};")

newString = headerString[:bodyIndex]

'''
# Find HTML body location in header
with open("include/NewStrings.h","a") as f:
    headerString = f.read()
    f.close()
'''

def writeString(htmlText, section):
    return "String "+section+" = {R\"=====(\n"+htmlText+"\n)=====};"

section = "css"
htmlText = "<body>\n</body>"
print(writeString(htmlText, section))