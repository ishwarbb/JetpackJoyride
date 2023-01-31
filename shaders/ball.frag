#version 330 core
in vec2 TexCoords;
in vec2 VertexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec2 Center;

// void main()
// {   
//     vec4 g = vec4(1.0,1.0,1.0,1.0);
//     float d = length(VertexCoords );

//     if(d < 4.0){
//         g = vec4(1.0,0.0,0.0,1.0);
//     }
     
//     color = g * vec4(spriteColor, 1.0) * texture(image, TexCoords);
// }  

// #version 330 core
// out vec4 FragColor;
  
// in vec2 TexCoords;

// uniform sampler2D image;
// uniform vec3 spriteColor;
  
// // uniform bool horizontal;
// uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

// void main()
// {             
//     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
//     vec3 result = texture(image, TexCoords).rgb * weight[0]; // current fragment's contribution
//     // if(horizontal)
//     // {
//         for(int i = 1; i < 5; ++i)
//         {
//             result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
//             result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
//         }
//     // }
//     // else
//     // {
//     //     for(int i = 1; i < 5; ++i)
//     //     {
//     //         result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
//     //         result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
//     //     }
//     // }
//     FragColor = vec4(1.0,1.0,1.0,1.0) * vec4(result, 1.0);
// }

// // ---------------------------------------------------------------------------
// // Fragment
// // ---------------------------------------------------------------------------
// #version 420 core
// //---------------------------------------------------------------------------
// in vec2 VertexCoords;                    // screen position <-1,+1>
// out vec4 FragColor;          // fragment output color
// uniform sampler2D image;          // texture to blur
// uniform float xs,ys;            // texture resolution
// uniform float r;                // blur radius
// //---------------------------------------------------------------------------
// void main()
//     {
//     float x,y,xx,yy,rr=r*r,dx,dy,w,w0;
//     w0=0.3780/pow(r,1.975);
//     vec2 p;
//     vec4 col=vec4(0.0,0.0,0.0,0.0);
//     for (dx=1.0/xs,x=-r,p.x=(0.25 + VertexCoords.x*0.5)+(x*dx);x<=r;x++,p.x+=dx)
//     { 
//         xx=x*x;
//         for (dy=1.0/ys,y=-r,p.y=(0.25 + VertexCoords.y*0.5)+(y*dy);y<=r;y++,p.y+=dy)
//         { 
//             yy=y*y;
//             if (xx+yy<=rr)
//             {
//                 w=w0*exp((-xx-yy)/(2.0*rr));
//                 col+=texture(image,p)*w;
//             }
//         }
//     }
//     FragColor=col;
// }

void main()
{
    vec2 iResolution = vec2(800.0,600.0);
    // vec2 Center = vec2(600,200);
    vec2 uv = gl_FragCoord.xy;
    vec2 pos = (uv - Center) / iResolution;
    pos.y /= iResolution.x/iResolution.y;

    float d = length(pos) * length(iResolution);
    float dist = 1/length(pos);
    
    //**********        Radius       **********
    
    // Dampen the glow to control the radius
    dist *= 0.1;
    
    //**********       Intensity     **********
    
    // Raising the result to a power allows us to change the glow fade behaviour
    // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
    // (Move the slider of m to see different fade rates)
    dist = pow(dist, 0.8);
    
    // Knowing the distance from a fragment to the source of the glow, the above can be 
    // written compactly as: 
    	// float getGlow(float dist, float radius, float intensity){
    	// 	return pow(radius/dist, intensity);
		// }
    // The returned value can then be multiplied with a colour to get the final result
       
    // if(dist >= 1.5)
    // {
        vec3 col = dist * vec3(1.0, 0.5, 0.25);
        // vec3 col = dist * vec3(1.0, 0.5, 0.25);


        // Tonemapping. See comment by P_Malin
        col = 1.0 - exp( -col );

        // Output to screen
        // color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
        // color = vec4(col, 1.0);
    // }
    // else
    // {
    //     color = vec4(0.0,0.0,0.0, 1.0);
    // }

    float c  = smoothstep(20,50,d);

    // color =  ((1 - c) * vec4(spriteColor, 1.0)) * texture(image, TexCoords) ;
    color =  ((1 - c) * vec4(1.0,1.0,1.0, 1.0)) ;
    if(d <= 45)
    {
        color +=  ((c) * vec4(1.0,1.0,0.0, 0.2)) ;
    }

}