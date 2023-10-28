#!/usr/bin/env python
import os

# Set the content type to HTML
content_type = "Content-Type: text/html\n"

# Create header for response
header = """
<!DOCTYPE html>
<html>
<head>
    <title>CGI POST Output</title>
</head>
<body>
"""

# Providing an inifnite loop to test time out in Cgi class
while True:
    answer = "<h1>Infinite Loop - This will run indefinitely</h1>"

# End the HTML document
footer = "</body></html>"

# Combine all parts into the response
response = header + answer + footer

# Send the response to the standard output
print(response)