var vShader = `
    uniform float time;
    varying vec3 fPosition;

    void main() {
        fPosition = position;
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }`;
