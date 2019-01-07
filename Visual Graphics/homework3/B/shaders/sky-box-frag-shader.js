var skyBoxFShader = `
    varying vec2 v_uv;
    varying vec3 v_pos;
    uniform samplerCube skyBox;

    void main() {
        gl_FragColor = textureCube(skyBox, v_pos);
    }`;
