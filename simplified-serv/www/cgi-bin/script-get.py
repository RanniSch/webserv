#!/usr/bin/env python
import time
import cgi
import os   # for environment variables

print("Hello World!")
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

env_vars = os.environ
for key, value in env_vars.items():
    if key != 'LC_CTYPE':        # Excludes the environment variable "LC_CTYPE: C.UTF-8" set by default. This way, only our user-defined variables are output. 
        print(f"{key}: {value}")

# print("</body>")
# print("</html>")