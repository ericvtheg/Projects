var texFShader = `
    varying vec2 v_uv;
    varying vec3 v_pos;
    uniform sampler2D texture;
    uniform vec3 size;
    void main() {

        if(v_pos.z == size.z * 0.5) {
            gl_FragColor = texture2D(texture, v_uv);
        }
        else {
            gl_FragColor = vec4(1.0,1.0,1.0,1.0);
        }
    }`;
