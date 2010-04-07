Spaceships is a top-down game where you have a spaceship that can shoot bullets at stuff.  Asteroids will fly 
at you randomly from around the screen.  You can shoot them.  If you collide with them, you will die (feature not done yet).  

To start:

Select "New Game" from the game menu and press enter.  

Controls:

a- turn left

d- turn right

w- move forward

space- shoot


f-fire secondary (currently the same plasma cannon as primary)
esc- exits the game (works from any screen as the message it generates is hard-coded)
arrow keys- navigate the menu screen



To build:

I built this in Visual Studio 2008 with DirectX 9.  All the lab computers have VS 2008 but I will attempt to create
a version of this in VS 2005 also (assuming that I can find my copy).  In order to build, you will need the following
things:



1.  all files should be in the same directory.

2.  ensure that you include the image files background.jpg, ships.jpg, and SPACE3.jpg in that same directory
3.  ensure that Visual Studio knows where to look for the headers and libraries for DirectX and windows. they are
typically installed (in Newton at least) in the following location: 

Headers:  
c:\Program Files\Microsoft DirectX SDK (November 2008)\Include

Libraries: c:\Program Files\Microsoft DirectX SDK (November 2008)\Lib\x86



To make them compile, in Visual Studio go to Tools->Options->Projects and Solutions->VC++ Directories.

Under "Show directories for:" select "include files" and then enter the directory listed above for headers.
Then select "Library Files" and include the directory listed above for Libraries.

I believe that I was able to fix or pragma away all warnings but if you get any you should just ignore them.



Known issues:

1.  Memory leaks- pretty sure I caught all of them but I haven't checked in a few weeks so no promises.  Should be minimal


2.  Collision decection- it's decent but not perfect and the collision radius on some objects was mostly guessing on 
my part.  The issue where you can collide with yourself and your own bullets is now fixed.  Also, spaceships are currently invincible (i.e. do not handle collissions properly).

3.  Composite on IRenderable- this is implemented but doesn't work correctly yet.  Strongly discourage you from using it at this time.  I'll get it working and release a patch

4.  Death blossom is not yet fully implemented.  The IVisitor components are also not yet fully tested, although the game works correctly if you are not using visitors.

5.  Sometimes asteroids flicker on the screen.  I believe that this is an issue with clipping in the graphics engine but I haven't had time to debug this yet but it doesn't affect gameplay at all.

