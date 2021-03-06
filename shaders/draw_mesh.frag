uniform float detail_tex_scale = 1.0;
uniform float snow_cov_amt = 0.0;
uniform sampler2D tex0, tex1;

//in vec2 tc; // comes from detail_normal_map.part.frag
in vec4 epos;
//in vec3 vpos, normal; // world space, come from indir_lighting.part.frag
//in vec3 eye_norm; // declared earlier

vec3 apply_bump_map(inout vec3 light_dir, inout vec3 eye_pos) {
	return apply_bump_map(light_dir, eye_pos, eye_norm, 1.0);
}

vec4 add_mesh_light0(in vec3 normal, in vec4 epos) {
	float ds_scale = 1.0;
#ifdef ENABLE_CLOUD_SHADOWS
	ds_scale = get_cloud_transparency(vpos, sun_pos);
#endif
	if (use_shadow_map) {normal *= get_shadow_map_weight_light0(epos, normal);}
	return add_light_comp_pos_scaled0(normal, epos, ds_scale, 1.0, gl_Color);
}
vec4 add_mesh_light1(in vec3 normal, in vec4 epos) {
	float ds_scale = 1.0;
#ifdef ENABLE_CLOUD_SHADOWS
	ds_scale = get_cloud_transparency(vpos, moon_pos);
#endif
	if (use_shadow_map) {normal *= get_shadow_map_weight_light1(epos, normal);}
	return add_light_comp_pos_scaled1(normal, epos, ds_scale, 1.0, gl_Color);
}

void main()
{
	vec3 lit_color = vec3(0.0);
	if (enable_light0) {lit_color += add_mesh_light0(eye_norm, epos).rgb;}
	if (enable_light1) {lit_color += add_mesh_light1(eye_norm, epos).rgb;}
	lit_color  = clamp(lit_color, 0.0, 1.0);
	add_indir_lighting(lit_color, 1.0);
	if (enable_dlights) {add_dlights(lit_color.rgb, vpos, epos, normalize(normal), vec3(1.0));} // dynamic lighting
	vec3 texel = texture(tex0, tc).rgb;

#ifdef MULT_DETAIL_TEXTURE // for mesh
	texel *= texture(tex1, 32.0*tc).rgb; // 32x scale
#endif
#ifdef ADD_DETAIL_TEXTURE // for water
	float slope_scale = clamp(50.0*(1.0 - normalize(normal).z), 0.0, 1.0); // normal.z is typically in +z
	texel *= vec3(1.0) + slope_scale*detail_tex_scale*texture(tex1, 32.0*tc).rgb; // 32x scale
	texel  = clamp(texel, 0.0, 1.0);
#endif

	lit_color   *= mix(texel, vec3(1.0, 1.0, 1.1), snow_cov_amt); // combine terrain texture with snow coverage
	fg_FragColor = vec4(lit_color, gl_Color.a);
#ifndef NO_FOG
	fg_FragColor = apply_fog_epos(fg_FragColor, epos);
#endif
}
