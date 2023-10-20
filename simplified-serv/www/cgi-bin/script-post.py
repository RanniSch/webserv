#!/usr/bin/env python
import os
import cgi

# Content-Type header definitions
print("Content-Type: text/html\n")

# Set file name and file content
filename = "new_file.txt"
file_content = "This is the content of the new file."

# Create file
with open(filename, 'w') as file:
    file.write(file_content)

# Send success message to server
print("The file '{}' was successfully generated.".format(filename))

# This is a small Python Skript that creates a file on the server.