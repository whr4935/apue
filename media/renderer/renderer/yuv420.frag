precision mediump float;
varying vec2 TexCoordOut;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
void main(void)
{
	vec3 yuv;
	vec3 rgb;
	yuv.x = texture2D(tex_y, TexCoordOut).r;
	yuv.y = texture2D(tex_u, TexCoordOut).r - 0.5;
	yuv.z = texture2D(tex_v, TexCoordOut).r - 0.5;
	rgb = mat3( 1,       1,         1,
	            0,       -0.39465,  2.03211,
	            1.13983, -0.58060,  0) * yuv;
	gl_FragColor = vec4(rgb, 1);
    //gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
