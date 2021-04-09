-- A simple scene with five spheres

mat1 = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 20)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.1, 0.1, 0.1}, 25)
mat3 = gr.material({0.3, 0.3, 0.3}, {0.8, 0.8, 0.8}, 100)
mat4 = gr.material({0.3, 0.4, 0.9}, {0.4, 0.4, 0.4}, 30)

mat5 = gr.material({0.7, 0.7, 0.3}, {0.5, 0.5, 0.4}, 30)

mat6 = gr.material({0.2, 0.7, 0.1}, {0.6, 0.6, 0.6}, 30)

scene_root = gr.node('root')

buckyball = gr.mesh( 'buckyball', 'buckyball.obj' )
scene_root:add_child(buckyball)
buckyball:set_material(mat2)

s5 = gr.nh_sphere('s5', {0, 50, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)

s6 = gr.nh_sphere('s6', {0, -100, -250}, 25)
scene_root:add_child(s6)
s6:set_material(mat3)

b1 = gr.nh_box('b1', {-100, 0, -400}, 100)
scene_root:add_child(b1)
b1:set_material(mat4)

s7 = gr.nh_sphere('s7', {-2, 2, 8}, 1)
scene_root:add_child(s7)
s7:set_material(mat5)

s8 = gr.nh_sphere('s8', {50, 0, -800}, 300)
scene_root:add_child(s8)
s8:set_material(mat6)

white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0.0})
magenta_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0.0})
side_light = gr.light({100, 100, 100}, {0.6,0.6,0.3}, {1,0.1,0.0})

gr.render(scene_root, 'sample.png', 500, 500,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {white_light, magenta_light, side_light})
