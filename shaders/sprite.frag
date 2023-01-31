#version 330 core
in vec2 TexCoords;
in vec2 VertexCoords;
out vec4 color;

uniform vec2 Center;
uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{   
        vec2 iResolution = vec2(800.0,600.0);
    // vec2 Center = vec2(400,400);
    vec2 uv = gl_FragCoord.xy;
    vec2 pos = (uv - Center) / iResolution;
    pos.y /= iResolution.x/iResolution.y;

    float dist = 1/length(pos);
    float d = length(pos) * length(iResolution);
    
    //**********        Radius       **********
    
    // Dampen the glow to control the radius
    dist *= 0.3;
    
    //**********       Intensity     **********
    
    // Raising the result to a power allows us to change the glow fade behaviour
    // See https://www.desmos.com/calculator/eecd6kmwy9 for an illustration
    // (Move the slider of m to see different fade rates)
    dist = pow(dist, 0.4);
    
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
        // color = vec4(col, 1.0);

    // vec4 yellow = vec4(1.0,0.886,0.129,1.0);
    // float d = VertexCoords.x;
    // float y = smoothstep(0,1,d); 

    // float c  = smoothstep(20,50,d);

    // color =  ((1 - c) * vec4(spriteColor, 1.0)) * texture(image, TexCoords) ;
    // color = vec4(col, 1.0);

    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}  