IN54-Project
============

To launch the project we need to compile OpenCV last version using CMake.
Compile the project in x64 using the CMakeLists file.

Enjoy :)


03/01/2015 - Yannis
I guess I can write here too, and in English so let's do it.
Today I've added face recognition using HaarCascade algorithm from OpenCV CascadeClassifier trained with pre-included data (OpenCV/sources/data/) that I copy-pasted into data/.
In output there are squares showing where are the found faces. The colors represent the state decided from Orientation in Face, but it's empty so right now it's plain random. The thickness show if the face has been recognized by several training bases ; the thicker it is, the more probable it's a face.

As a reminder there are 2 things to complete:
 - OrientationDetector1, OrientationDetector2 etc. for each algorithm managing the orientation extraction, from a face bounding box (and the image obviously).
 - DecisionTaker for determining the state of a face (looking at the camera / not looking at the camera) from face bounding box, orientation and whatever.

Everything can be changed if needed.
