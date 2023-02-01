#version 330 core
in vec2 TexCoords;
in vec2 VertexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec2 Center;
uniform vec2 end1;
uniform vec2 end2;

// void main()
// {
//     vec2 iResolution = vec2(800.0,600.0);

//     vec2 uv = gl_FragCoord.xy;
//     vec2 pos = (uv -Center) / iResolution;
//     pos.y /= iResolution.x/iResolution.y;

//     float dist = 1/length(pos);
    
//     //**********        Radius       **********
    
//     // Dampen the glow to control the radius
//     dist *= 0.3;
    
//     //**********       Intensity     **********
    
//     // Raising the result to a power allows us to change the glow fade behaviour
//     // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
//     // (Move the slider of m to see different fade rates)
//     dist = pow(dist, 0.8);
    
//     // Knowing the distance from a fragment to the source of the glow, the above can be 
//     // written compactly as: 
//     	// float getGlow(float dist, float radius, float intensity){
//     	// 	return pow(radius/dist, intensity);
// 		// }
//     // The returned value can then be multiplied with a colour to get the final result
       
//     // if(dist >= 1.5)
//     // {
//         vec3 col = dist * vec3(1.0, 0.5, 0.25);
//         // vec3 col = dist * vec3(1.0, 0.5, 0.25);


//         // Tonemapping. See comment by P_Malin
//         col = 1.0 - exp( -col );

//         // Output to screen
//         color = vec4(col, 1.0);
//     // }
//     // else
//     // {
//     //     color = vec4(0.0,0.0,0.0, 1.0);
//     // }

//     // float c  = smoothstep(30,50,dist);

//     // color =  (c) * vec4(spriteColor, 1.0);
// }


void main()
{
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    // vec2 end1 = vec2(200,200);
    // vec2 end2 = vec2(600,600);

    float x1 = end1.x;
    float x2 = end2.x;
    float y1 = end1.y;
    float y2 = end2.y;

    float a = y2 - y1;
    float b = x1 - x2;
    float c0 = (x2*y1) - (x1*y2);
    float normaliser = sqrt(a*a + b*b);

    float d = (a*x + b*y + c0) / normaliser;

    // float d = length(end1 - gl_FragCoord.xy);
    // d = abs(d);
    if(d < 0)
    {
        d = d * -1;
    }

    float c  = smoothstep(5,20,d);

    if(d > 20)
    {
        discard;
    }

    // color =  ((1 - c) * vec4(spriteColor, 1.0)) * texture(image, TexCoords) ;
    color =  (1 - c) * vec4(1.0,1.0,1.0, 1.0) + c * vec4(1.0,1.0,0.0, 0.7) ;
    // color =   vec4(1.0,1.0,1.0, 1.0);
}

// #version 330 core
// in vec2 TexCoords;
// in vec2 VertexCoords;
// out vec4 color;

// uniform vec2 Center;
// uniform sampler2D image;
// uniform vec3 spriteColor;
// uniform vec2 end1;
// uniform vec2 end2;

// void main()
// {   
//         vec2 iResolution = vec2(800.0,600.0);
//     // vec2 Center = vec2(400,400);
//     vec2 uv = gl_FragCoord.xy;
//     vec2 pos = (uv - Center) / iResolution;
//     pos.y /= iResolution.x/iResolution.y;

//     float dist = 1/length(pos);
//     float d = length(pos) * length(iResolution);
    
//     //**********        Radius       **********
    
//     // Dampen the glow to control the radius
//     dist *= 0.3;
    
//     //**********       Intensity     **********
    
//     // Raising the result to a power allows us to change the glow fade behaviour
//     // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
//     // (Move the slider of m to see different fade rates)
//     dist = pow(dist, 0.4);
    
//     // Knowing the distance from a fragment to the source of the glow, the above can be 
//     // written compactly as: 
//     	// float getGlow(float dist, float radius, float intensity){
//     	// 	return pow(radius/dist, intensity);
// 		// }
//     // The returned value can then be multiplied with a colour to get the final result
       
//     // if(dist >= 1.5)
//     // {
//         vec3 col = dist * vec3(1.0, 0.5, 0.25);
//         // vec3 col = dist * vec3(1.0, 0.5, 0.25);


//         // Tonemapping. See comment by P_Malin
//         col = 1.0 - exp( -col );

//         // Output to screen
//         // color = vec4(col, 1.0);

//     // vec4 yellow = vec4(1.0,0.886,0.129,1.0);
//     // float d = VertexCoords.x;
//     // float y = smoothstep(0,1,d); 

//     // float c  = smoothstep(20,50,d);

//     // color =  ((1 - c) * vec4(spriteColor, 1.0)) * texture(image, TexCoords) ;
//     // color = vec4(col, 1.0);

//     color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
// }  