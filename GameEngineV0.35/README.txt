Group Members:
 -Robert Onulak
 -Westley Hennigh
 -Wylder Keane

Overview:
-We ended up using Chris Peter's component engine and writing the game from nearly scratch,
so as you'll notice there are some significant differences to the program from the original.
The object of the game is to gather 5000 points. You do this by shooting asteroids with your
plasma cannon. You can't damage other players or get damaged by the asteroids, however if you
hit them they disappear and you don't gain points.

Config File:
-The config file (located in the Data folder) specifies the user name and the server IP
address and port. It also specifies a range of UDP ports that can be used. The networking
system will not allow more UDP sockets to be created beyond the ones in that range at one time.
However, once a socket is closed, that port becomes available again.

Usage:
-Open the solution file as you would and just compile. It should just build and run without any
special changes or input.

Quirks / Extra Info:
-If you attempt to connect to a server and you get no response it will leave a blank screen and wait
forever.