#!/usr/bin/env python
import os
import cgi

# Set the content type to HTML
content_type = "Content-Type: text/html\n"

# Create header for response
header = """
<!DOCTYPE html>
<html>
<head>
    <title>CGI Output</title>
</head>
<body>
"""

# Create an instance of the FieldStorage class to process form data
form = cgi.FieldStorage()

# Process form data, if available (import cgi)
form_data = ""
if 'form_field' in form:
    form_data = form['form_field'].value

# Read input from CGI environment (the message body)
input_data = os.environ.get("QUERY_STRING") # Name = Body

if input_data == "Hi":
    answer = "<html><body><h1>Hi, how are you?</h1></body></html>"
else:
    answer = "<html><body><h1>Sad, that you did not write hi!</h1></body></html>"

# End the HTML document
footer = "</body></html>"

# Combine all parts into the response
response = header + answer + footer

# Saves answer in environment
#os.environ['MY_CGI_RESPONSE'] = response    # for this char* response = getenv("MY_CGI_RESPONSE"); in Cgi.cpp needed

# Send the response to the standard output
print(response)