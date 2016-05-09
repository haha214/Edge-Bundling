#extension GL_EXT_gpu_shader4 : enable
float PI=3.1415926f;
float _radiusEquator=6378137f;
float _radiusPolar = 6356752.3142;

varying vec4 color;
double flattening = (_radiusEquator-_radiusPolar)/_radiusEquator;
double _eccentricitySquared = 2*flattening - flattening*flattening;
	vec3 convertLatLongHeightToXYZ(vec3 llh)
	{
       double sin_latitude = sin(llh.y/180.0*PI);
       double cos_latitude = cos(llh.y/180.0*PI);
       double N = _radiusEquator / sqrt( 1.0 - _eccentricitySquared*sin_latitude*sin_latitude);
		vec3 XYZ;
		XYZ.x = (N+llh.z)*cos_latitude*cos(llh.x/180.0*PI);
		XYZ.y = (N+llh.z)*cos_latitude*sin(llh.x/180.0*PI);
		XYZ.z = (N*(1-_eccentricitySquared)+llh.z*5000)*sin_latitude;
		return XYZ;
	}
void main(void)
{
color = gl_Color;
    vec4 pos=gl_Vertex;
    pos.xyz = convertLatLongHeightToXYZ(pos.xyz);	
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}


