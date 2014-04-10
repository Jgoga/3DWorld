uniform float smoke_bb[6]; // x1,x2,y1,y2,z1,z2
uniform mat4 world_space_mvm;
uniform float tex_scale_s  = 1.0;
uniform float tex_scale_t  = 1.0;
uniform vec3 world_space_offset = vec3(0.0);
uniform vec3 sun_pos; // used for dynamic smoke shadows line clipping

attribute vec4 tex0_s, tex0_t;

varying vec3 eye, vpos, normal, lpos0, vposl; // world space
// epos, eye_norm, and tex_coord come from bump_map.vert

void main()
{
	if (use_texgen == 1) {
		setup_texgen_st();
		tex_coord = tc;
	}
	else if (use_texgen == 2) {
		tex_coord = vec2(dot(fg_Vertex, tex0_s), dot(fg_Vertex, tex0_t));
	}
	else if (use_texgen == 3) {
		set_tc0_from_vert_id();
		tex_coord = tc;
	}
	else {
		tex_coord = fg_TexCoord * vec2(tex_scale_s, tex_scale_t);
	}
	gl_Position   = fg_ftransform();
	gl_FrontColor = fg_Color;
	eye_norm      = normalize(gl_NormalMatrix * fg_Normal);
	epos          = gl_ModelViewMatrix * fg_Vertex;

	if (use_world_space_mvm) {
		normal = normalize((transpose(world_space_mvm) * vec4(eye_norm, 1)).xyz);
		mat4 mvm_inv = inverse(world_space_mvm);
		vpos   = (mvm_inv * epos).xyz; // world space
		eye    = mvm_inv[3].xyz; // world space
	}
	else {
		vpos   = fg_Vertex.xyz + world_space_offset;
		eye    = gl_ModelViewMatrixInverse[3].xyz; // world space
		normal = normalize(fg_Normal);
	}
#ifdef USE_BUMP_MAP
	setup_tbn();
#endif

#ifdef DYNAMIC_SMOKE_SHADOWS
	pt_pair res2 = clip_line(vpos, sun_pos, smoke_bb);
	lpos0 = res2.v1;
	vposl = res2.v2;
#endif
} 
