A simple ray tracer implemented in C++.


COMPILATION:
		cd A4
		premake4 gmake
		make
		cd Assets
		../A4 sample.lua

Made as an assignment submission for a final year undergraduate graphics course.

Manual:

	I completed all objectives except the hierarchical transformations.
	I decided to not do the hierarchical transformations since I have many assignments on the go and I needed to save time.
	I added no extra commands and generated no extra data files.

	I used the blinn-phong model for lighting.
	As my extra objective I implemented supersampling.
	I also implemented attenuation.
	I added attenuation coefficients to one of my lights in sample.lua to show it.
	My sample.lua is simple but I tried to structure it to show highlights and shadows and all 3 types of primitive.

	I used the course notes, supplemental notes and Fundamentals of Computer Graphics extensively to do this assignment.
	I included an extra rendering of a cow I did since the all the files with the cow models were hierarchical
