#!/usr/bin/python3
import re, fnmatch

# Extract HTML file body 
with open("lib/nav.html","r") as f:
    htmlString = f.read()
    f.close()

bodyIndex = [htmlString.find("<body>"),htmlString.find("</body>")]
htmlBody = htmlString[bodyIndex[0]:bodyIndex[1]]

# Find HTML body location in header
with open("include/TestStrings.h","r") as f:
    headerString = f.read()
    f.close()

bodyIndex = headerString.find("String bodyString = {R\"=====(\n)=====\"};")
print(headerString[bodyIndex]+30)

newString = 

'''
# Find HTML body location in header
with open("include/NewStrings.h","a") as f:
    headerString = f.read()
    f.close()
'''
