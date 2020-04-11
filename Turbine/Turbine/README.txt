
## ================================================
## 	--= Spinout Ride =--

## ================================================
# Notes

As the specification for this assignment requires different
forms of animation interaction with the ride, I have added in
some different animations:

 - Manual controll of individual ride componenets
 - Scripted animation with start-stop on key press
 - Scripted animation of a full ride, which starts and stops itself

My extended features have been mainly implemented into this third
animation type. My main extended feature is the addition of particle
effects into the scene, which required instancing, billboarding and
transparency to be implemented into the solution. The particle effects are 
seen as the ride generating smoke and flames. These effects can either be 
triggered manually, or occur as part of the scripted animation of the 
full ride.

I have used these tools in other areas of the project as well, such as 
Instancing being used to generate the many primitive trees in the scene.



## ================================================
# Key controlls

# Manual Ride controll
Platter clockwise.................. LEFT KEY OR NUM 1
Platter anti-clockwise............. RIGHT KEY OR NUM 3
Arm clockwise...................... NUM 7
Arm anti-clockwise................. NUM 9
Seats clockwise.................... NUM 4
Seats anti-clockwise............... NUM 6
Tilt up............................ UP KEY OR NUM 5
Tilt down.......................... DOWN KEY OR NUM 2
Reset all rotations................ NUM 8

# Animation
Toggle default animation on/off.... ENTER
Preset full ride animation......... R
Preset full ride animation (long).. L

# Navigation
Look around (Arcball).............. MOUSE L
Pan................................ MOUSE M
Look around (Fixed)................ MOUSE R
Switch POV and world camera........ SPACE

# Other
Manual smoke....................... G
Manual fire........................ F
Toggle day/night................... D
