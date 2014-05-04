# Computer Vision Theremin

A theremin (musical instrument) created using OpenCV and the Synthesis Toolkit (STK). The real instrument uses disturbance in the electrical field or something (TODO give physics degree back) to measure hand proximity and produce a note, but we can get pretty close using computer vision techniques to track a user's hand with the camera.

## License
GPLv3

## How it works
After trying a few techniques (Haar cascade, Farneback, back projection) I found that Lucas Kanade was the most robust at tracking a naked human hand. However, it suffered from 'stickiness' whereby the tracked points would get stuck on background objects.

To resolve this, I extended the algorithm to dynamically prune and spawn points. Points that don't move within a few frames are killed. When they die, a set of new points is spawned in a star pattern around the dead one. This is essentially a way of making a grab for the tracked item before it gets away.

This was effective at keeping hold of a hand moving at a reasonable speed. I created some tests to try and measure the effectiveness of various optimizations to the algorithm. Producing a note based on hand position is easy. Playing an instrument using hand gestures without physical feedback, however: really damn hard.
