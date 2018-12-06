var reflectiveVShader = `

    varying vec3 v_reflected;

    void main() {

        // Calculate position in the world coordinate system
        vec4 worldPosition = modelMatrix * vec4(position, 1.0);

        // Calculate normal in the world coordinate system
        vec4 worldNormal = modelMatrix * vec4(normal, 0.0);

        // Calculate the incident vector
        vec3 incident = worldPosition.xyz - cameraPosition;

        // Calculate the reflected vector
        v_reflected = reflect(incident, normalize(worldNormal.xyz));

        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
`;
