# WEBSERVER ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54) ![HTML5](https://img.shields.io/badge/html5-%23E34F26.svg?style=for-the-badge&logo=html5&logoColor=white)
Webserver is a 42 Wolfsburg curriculum project which is written using C++, Python for CGI scripts and HTML for basic webpages.

## What is Webserver?
The project is about writing an HTTP server in C++ 98 and learning about the primary function of a web server, which is to store, process and serve webpages to to clients. Communication between client and server is done using the Hypertext Transfer Protocol (HTTP) --> listening to clients **HTTP** requests for files and returning them back to the client. <br>
The web browser initiates communication by requesting a specific resource using HTTP, and the server responds with the content of that resource or an error message if it is unable to do so.

## How does the Client acces the webserver?
Client can access the webserver using a localhost and the port that is specified by the webserver. For it to work the webserver first has to be running in the background.

## How to run the webserver
* Clone the repository ```git clone git@github.com:RanniSch/Webserv.git ./the_name_of_your_choice```
* ```cd``` into the repository ```cd the_name_of_your_choice```
* ```cd``` into the simplified-serv ```cd simplified-serv```
* Run the command ```make```
* Run webserver ```./webserv [path of config file]```
* Open the browser and type in **localhost:8000**
* You can test different scenarios by **localhost:8000/[different scenarios]** f.e. **localhost:8000/upload.html**

## Prerequisites
- The server is serving a fully static website
- The GET, POST and DELETE methods are implemented.
- Clients can upload files.
- The HTTP response status codes are correct
- The server has default error pages if none are provided
- The server can listen on multiple ports
- The server is compatible with the Firefox and Chrome web browsers.
- The program takes a configuration file as an argument or uses a default path
- The program is non-blocking and uses only 1 poll() (or equivalent) for all I/O operations between the client and the server (including listening).
- poll() checks reads and writes at the same time
- Every read or write goes through poll()
- A request to the server will never hang forever
- NGINX is HTTP 1.1 compliant

## Configuration File
In the configuration file, you can
- Choose the port and host of each 'server'.
- Set the server_names or not
- Set default error pages
- Limit client body size
- Set up routes with one or more rules/configurations 

Various configuration files and default base files for testing and demonstrating each feature are provided.

![webserv-1](https://github.com/RanniSch/webserv/assets/104382315/67e54924-a84e-4884-a438-633f1cd006d4)


![webserv-2](https://github.com/RanniSch/webserv/assets/104382315/edcf7a70-c874-4720-8a17-9d0ed10f6031)

![webserv-3](https://github.com/RanniSch/webserv/assets/104382315/2335a842-0efc-4dc1-96e0-302da2a8368c)

![webserv-4](https://github.com/RanniSch/webserv/assets/104382315/0272b978-371c-4900-b47e-a368ea1d6f1b)


![webserv-5](https://github.com/RanniSch/webserv/assets/104382315/dd955b01-b31b-4bbe-9e70-9ab85af1f3a5)
