The application creates a 3d virtual enviroment that responds to aspects of the audio tracks
The hi-hats kicks, 808 basses and percs can be controlled via XY coordinates of the viewers mouse
The 4 centre spheres are tracked to the volumes of these components
effects are time based and as the track progresses the lighting, object orientation and movements change.

The source "landscape" of the model is : 
3D LANDSCAPE - "Dead Sea Qumran" (https://skfb.ly/6rUDB) by GSXNet is licensed under Creative Commons Attribution-NonCommercial (http://creativecommons.org/licenses/by-nc/4.0/).

The 3 wav audio tracks are created by me 

the cuboid wall is created using nested loops, arranging them in a grid.
	- They are manipulated at random, the extent of this is based on kick volume
	- They are then expanded into 3d depth 65 seconds into the track.
the spheres that appear later into the track are tracked to the volumes controlled by the mouse coordinates
	- Later "gravity" is increased, mapped to the songs composition


The following techniques have been used:
	- Interpolation (glm::mix, ofLerp)
	-sine-wave BPM modulation
	-randomised axis scaling
	-audio amplitude mapping
	-timed scene transitions

OfApp.cpp layout:

setup()
	initialise audio, model, cubes and lighting
update()
	Handle audio interaction and physics of the 3D scene
draw()
	Render scene, lighting and animations
drawWithModel()
	Render animated quarry model



