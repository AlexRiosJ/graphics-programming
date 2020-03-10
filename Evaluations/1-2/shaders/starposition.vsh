in vec2 vertexPosition;
in vec3 vertexColor;
in int vertexFactor;

uniform float factorEx;
uniform float factorIn;

out vec3 vertexColorToFS;

void main(){
    if(vertexFactor == 0){
        gl_Position = vec4(vec3(vertexPosition.x*factorIn, vertexPosition.y*factorIn,0), 1);
    }else{
        gl_Position = vec4(vec3(vertexPosition.x*factorEx, vertexPosition.y*factorEx,0), 1);
    }
    
    vertexColorToFS = vertexColor;
}