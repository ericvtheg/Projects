var fShader = `
    uniform float time;
    varying vec3 fPosition;
    void main() {
        gl_FragColor = vec4(0.0,0.0,fPosition.z,1.0);
    }`;
