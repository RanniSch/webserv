#!/usr/bin/env python
import time
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

# Process form data, if available
form_data = ""
if 'form_field' in form:
    form_data = form['form_field'].value

# Detect actual date and time
current_datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
date_time_output = f"<h1>Current Date and Time</h1><p>{current_datetime}</p>"

# Create HTML for environment variables
env_vars = os.environ
env_output = "<h1>Environment Variables</h1><table border='1'><tr><th>Variable</th><th>Value</th></tr>"
for key, value in env_vars.items():
    if key != 'LC_CTYPE':
        env_output += f"<tr><td>{key}</td><td>{value}</td></tr>"
env_output += "</table>"

# End the HTML document
footer = "</body></html>"

# Combine all parts into the response
response = header + date_time_output + env_output + footer

# Send the response to the standard output
print(response)