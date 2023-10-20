# CPSC441-Proxy

Name: Bartu Okan

This program acts as the middleman between the client and the server, creating a proxy for HTTP protocols.
When it is run and connection is routed through localhost:80, it will modify the responses from the servers, changing every word "frog" into "Fred" and replace all images with extension ".jpg" to a set image.
It was tested on a linux virtual machine and can be run on linux machines using the command line.

To Compile: g++ -o proxy proxy.cpp
To Run: ./proxy

Also need to change your browser's proxy settings to route the traffic through the localhost:80.
On Firefox, this can be done by going to Preferences -> Network Settings -> Configure how Firefox connects to internet
Then choosing the Manual proxy configuration option and setting the HTTP Proxy IP Address to 127.0.0.1 and port to 8080.
Reminder: This proxy only works for HTTP Protocols. HTTPS or FTP will not work.

 
