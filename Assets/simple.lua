-- A simple scene with five spheres
mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({0.6, 0.1, 0.1}, {0.1, 0.1, 0.1}, 25)

scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {180, 180, 70}, 30)
scene_root:add_child(s1)
s1:set_material(mat1)


s4 = gr.nh_box('s4', {-200, -250, 50}, 400)
scene_root:add_child(s4)
s4:set_material(mat3)


white_light = gr.light({-100.0, 350.0, 300.0}, {0.9, 0.9, 0.9}, {1, 0, 0.0})
magenta_light = gr.light({-100.0, 200.0, 100.0}, {0.2, 0.2, 0.4}, {1, 0, 0.0})

gr.render(scene_root, 'simple.png', 768, 768,
	  {0, 400, 800}, {0, 0, -800}, {0, 1, 0}, 50,
	  {0.3, 0.3, 0.3}, {white_light, magenta_light})
