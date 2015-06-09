#version 410 core

uniform sampler2D front;
uniform sampler3D volume;

uniform float minTF = 0.0;
uniform vec3 domainSize = vec3(400,296,352);


// INPUT VARIABLES
in vec2 texcoord;
in vec3 normalview;
in vec4 screenspaceposition;

// OUTPUT VARIABLES
out vec4 outputColor;


uint Hash(uvec4 bd);
uint Serialize(uvec4 bd);

uint AccessHashTable(uint hashValue, uint serializedValue);

//get the fake-normal
vec3 calculateNormal(vec3 texcoord, float isoValue){
  vec3 stepSize = vec3(1.0,1.0,1.0)/domainSize;
  
  float c = 0;
  vec3 sampleOffset;
  
  vec3 normal = vec3(0,0,0);
  
  for(float x = -1.0; x <= 1.0;++x){
    for(float y = -1.0; y <= 1.0;++y){
      for(float z = -1.0; z <= 1.0;++z){
        
        if(!(x == 0 && y == 0 && z == 0)){
          sampleOffset = vec3(x,y,z)*stepSize;
          c = texture(volume,texcoord+sampleOffset).x;
          if(c < isoValue){
            vec3 n = vec3(x,y,z);
            
            normal += normalize(n);
          }
        }
        
      }
    }
  }
  normal = normalize(normal);
  return normal;
}


// Pixel Shader
void main(void)
{
  //calculate the texture position of the fragment
  vec2 pos = ((screenspaceposition.xy/screenspaceposition.w)+1.0)/2.0;
  
  // read the front face texture for entry pointin volume
  vec3 frontT = texture(front, pos).xyz;
  
  //! direction of ray \todo change normalview out
  vec3 dir = normalview-frontT;
  dir = normalize(dir);
  
  //final fragment color
  vec4 fragmentColor = vec4(0,0,0,0);
  
  //value in volume
  float volumeValue = texture(volume,frontT).x;
  
  //current position inside the volume
  vec3 textpos = frontT;
  
  //max steps, make uniform later
  float maxTest = 1000.0;
  
  //step size
  float step = 1.0/float(maxTest)*2;
  
  //minvalue aka minTF -> later transfer function
  float minValue = minTF;
  

  for(int i = 0; i < maxTest; i++){
    
    //check if inside the volume
    if(textpos.x >= 0.0 && textpos.x <= 1.0 &&
       textpos.y >= 0.0 && textpos.y <= 1.0 &&
       textpos.z >= 0.0 && textpos.z <= 1.0){
      
      volumeValue = texture(volume,textpos).x;
      
      
      if(volumeValue >= minValue){
        i = int(maxTest);
        
        
        vec3 normal = calculateNormal(textpos,volumeValue);
        
        vec3 lightpos = vec3(10,5,-10);
        float radiance = dot(normal,normalize(lightpos));
        
        fragmentColor.x = radiance;
        fragmentColor.y = radiance;
        fragmentColor.z = radiance;
        fragmentColor.w = 1;
        break;

      }
      
      textpos += (dir*step);
    }else{
      i = int(maxTest);
      break;
    }
    
  }

  
  
  outputColor = fragmentColor;
  
  //outputColor = vec4(frontT,1);
}
