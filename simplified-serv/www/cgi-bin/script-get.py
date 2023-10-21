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
    print(f"{key}: {value}")

# variable1 = os.getenv("testVariable")
# print("Umgebungsvariable 1:", variable1)

# variable_value = os.getenv("MY_TEST_1")
# print("MY_TEST_1:", variable_value)
print("MY_TEST_1:", testVariable)

# for key, value in os.environ.items():
#    print(f"{key}={value}")

# print(os.environ.get('a'))    # sucht a von env raus

# print("</body>")
# print("</html>")