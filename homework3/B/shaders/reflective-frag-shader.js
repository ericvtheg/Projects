var reflectiveFShader = `
    varying vec3 v_reflected;
    uniform samplerCube skyBox;

    void main() {
        gl_FragColor = textureCube(skyBox, v_reflected);
    }
`;
