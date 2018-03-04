var sdfVShader = `
        varying vec3 v_pos;
        void main() {
            v_pos = position;
            gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
        }
    `;

var sdfFShader = `
        const int MAX_MARCHING_STEPS = 300;
        const float EPISOLON = 0.0001;
        const float START = 0.0;
        const float END = 300.0;

        uniform vec2 resolution;
        varying vec3 v_pos;

        float sphereSDF(vec3 point) {
            return(length(point) - 1.0);
        }

        float boxSDF(vec3 point, vec3 size) {
          return length(max(abs(point)-size,0.0));
        }

		float sdTorus( vec3 p, vec2 t ){
			vec2 q = vec2(length(p.xz)-t.x,p.y);
			return length(q)-t.y;
		}
        // Cheated and copied from shader toy example:
        // https://www.shadertoy.com/view/lt33z7
        vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
            vec2 xy = fragCoord;
            float z = size.y / tan(radians(fieldOfView) / 2.0);
            return normalize(vec3(xy, -z));
        }

        // Cheated and copied from shader toy example:
        // https://www.shadertoy.com/view/lt33z7
        mat3 rayMarchViewMatrix(vec3 cam, vec3 center, vec3 up) {
            // Based on gluLookAt man page
            vec3 f = normalize(center - cam);
            vec3 s = normalize(cross(f, up));
            vec3 u = cross(s, f);
            return mat3(s, u, -f);
        }

        float rayMarchCube(vec3 cam, vec3 dir, float start, float end) {
            float step = start;
            for(int i = 0; i < MAX_MARCHING_STEPS; i++) {
                float dist = boxSDF(cam + step * dir, vec3(0.675,0.675,0.675));
                if(dist < EPISOLON) {
                    // I am inside the geometry
                    return step;
                }

                step += dist;
                if(step >= end) {
                    return end;
                }
            }
			

            return end;
        }
		
        float rayMarchSphere(vec3 cam, vec3 dir, float start, float end) {
            float step = start;
			
            for(int i = 0; i < MAX_MARCHING_STEPS; i++) {
                float dist = sphereSDF(cam + step * dir);
                if(dist < EPISOLON) {
                    // I am inside the geometry
                    return step;
                }

                step += dist;
                if(step >= end) {
                    return end;
                }
            }
            return end;
        }
/* Union of shapes */
		float opU( float d1, float d2 ){
			return min(d1,d2);
		}
		/*
		float sdf_blend(float d1, float d2, float a){
			float a = a * d1;
			a = (1 - a);
			a *= d2;
			return a;
		}*/
        void main() {
            vec3 cam = vec3(8.0,5.0,7.0);
            vec3 dir = rayDirection(50.0, resolution, v_pos.xy);
            mat3 viewToWorld = rayMarchViewMatrix(cam, vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
            vec3 worldDir = viewToWorld * dir;
			float dist2 = rayMarchSphere(cam, worldDir, START, END);
			float dist1 = rayMarchCube(cam, worldDir, START, END);
			float dist = opU(dist1, dist2);
            if(dist > END - EPISOLON) {
                gl_FragColor = vec4(0.0,0.0,0.0,0.0);
                return;
            }
			
            gl_FragColor = vec4(1.0,1.0,0.0,0.0);
			
			/*float sdf = sdf_blend(dist1, dist2, 0.2);*/
        }
    `;
