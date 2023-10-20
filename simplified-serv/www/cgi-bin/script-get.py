#!/usr/bin/env python
import time
import cgi

# print("<!DOCTYPE html>")

# Detect actual date and time
current_datetime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())

# print("<html>")
# print("<head><title>CGI Example</title></head>")
# print("<body>")

# Print Content-Type-Header
print("Content-Type: text/plain\n")

# Print actual date and time
print(current_datetime)

# print("</body>")
# print("</html>")