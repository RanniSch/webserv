#!/usr/bin/env python
import time
import cgi
import os   # for environment variables

# Set the content type to HTML
print("Content-Type: text/html\n")

# Start the HTML document
print("<html>")
print("<head>")
print("<title>CGI Output</title>")
print("</head>")
print("<body>")

# Detect actual date and time
current_datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())

# Print actual date and time
print("<h1>Current Date and Time</h1>")
print(f"<p>{current_datetime}</p>")

# Print environment variables in an HTML table
print("<h1>Environment Variables</h1>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")

env_vars = os.environ
for key, value in env_vars.items():
    if key != 'LC_CTYPE':        # Excludes the environment variable "LC_CTYPE: C.UTF-8" set by default. This way, only our user-defined variables are output. 
        print(f"<tr><td>{key}</td><td>{value}</td></tr>")

print("</table>")

# End the HTML document
print("</body>")
print("</html>")