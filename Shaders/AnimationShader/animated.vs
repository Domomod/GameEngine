#version 420 core

layout( location = 0 ) in vec4 position;
layout( location = 1 ) in vec2 texCoord;
layout( location = 2 ) in ivec4 boneIDs;
layout( location = 3 ) in vec4 weights;

smooth out vec2 outTexCoord;

const int MAX_BONES = 50;

layout (std140, binding = 2) uniform block{
mat4 ModelViewProjection;
mat4 bones[MAX_BONES];
};

void main(){
    vec4 localPos = vec4(0.0);

    	for(int i=0;i<4;i++){
    		mat4 boneTransform = bones[boneIDs[i]];
    		vec4 posePosition = boneTransform * position;
    		localPos += posePosition * weights[i];

    }
    outTexCoord = texCoord;
    gl_Position = ModelViewProjection * localPos;
}