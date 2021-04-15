# NI-PG1
 
Basic raytracing CPU renderer implementation for NI-PG1. It was developed on **Windows** so it uses c++ windows libraries.

## About

Ray tracer utilizes basic implementation of light ray which reflect and refracts. It can load obj and modified txt files for information about camera and light. It supports area lights which samples by default 32 lights / triangle (can be set in ```consts.h```). For increased speed I implemented basic BVH structure, which sped up rendering a lot of times.

## Renders

Render of scene<br>
![Render](doc/5sp-8min.jpg)
