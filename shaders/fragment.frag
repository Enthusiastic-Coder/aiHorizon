uniform sampler2D texture0;
uniform mediump vec3 cameraPosition;
uniform mediump vec3 lightPos;

varying mediump vec3 outNormal;
varying mediump vec3 outTangent;
varying mediump vec3 outColor;
varying mediump vec2 outUV;
varying mediump vec3 outLightPos;
varying mediump vec3 position;

const mediump vec3 ambientColor=vec3(0.05,0.05,0.05);
mediump vec3 diffuseColor=vec3(0.7,0.7,0.7);
const mediump vec3 specularColor=vec3(1.0,1.0,1.0);

void main()
{
     mediump vec3 normal=normalize(outNormal);
     mediump vec3 surf2light=normalize(lightPos-position);
     mediump vec3 surf2camera=normalize(-position);
     mediump vec3 reflection=-reflect(surf2camera,normal);
     mediump float diffuseContribution=max(0.0,dot(normal,surf2light));
     mediump float specularContribution=pow(max(0.0,dot(reflection,surf2light)),4.0);
	
    diffuseColor = texture2D(texture0,outUV).rgb;

    gl_FragColor=vec4(ambientColor*outColor+ diffuseContribution *outColor,1.0);
                    //+ specularContribution*specularColor,1.0);
}
