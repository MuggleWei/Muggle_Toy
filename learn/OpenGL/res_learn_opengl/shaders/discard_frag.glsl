#version 400

in vec3 FrontColor;
in vec3 BackColor;
in vec2 TexCoord;

layout( location = 0 ) out vec4 FragColor;

void main()
{
	bvec2 toDiscard = greaterThan( TexCoord, vec2(0.0,0.65) );
	
	if( all(toDiscard) )
		discard;

	if( gl_FrontFacing ) 
	{
		FragColor = vec4(FrontColor, 1.0);
	} 
	else 
	{
		FragColor = vec4(BackColor, 1.0);
	}
}