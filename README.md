# NI-PG1
 
Basic raytracing CPU renderer implementation for NI-PG1. It was developed on **Windows** so it uses c++ windows libraries.

## About

Ray tracer utilizes basic implementation of light ray which reflect and refracts. It can load obj and modified txt files for information about camera and light. It supports area lights which samples by default 32 lights / triangle (can be set in ```consts.h```). For increased speed I implemented basic BVH structure, which sped up rendering a lot of times.

## Renders

Render of scene with 1 bounce and 32 sample area light. It took 21 minutes.<br>
![Render](doc/1sp%2021min.jpg)


Render of scene with 3 bounces and 32 sample area light. It took 27 minutes.<br>
![Render](doc/3sp%2027min.jpg)


Render of scene with 10 bounce and 32 sample area light. It took 28 minutes.<br>
![Render](doc/10sp%2028min.jpg)
