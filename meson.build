project('SDL2/OpenGL', 'cpp', version: '0.2.0',
    default_options: ['cpp_std=c++23'],
    meson_version: '>=1.6.0')
incdir = include_directories('Bengine/header/')

srcdir = files(
    'main.cpp',
    'MainGame.cpp',
    'Agent.cpp',
    'Human.cpp',
    'Zombie.cpp',
    'Player.cpp',
    'Level.cpp',
    'Weapon.cpp',
    'Bengine/source/MainWindow.cpp',
    'Bengine/source/Error.cpp',
    'Bengine/source/IOManager.cpp',
    'Bengine/source/ResourceManager.cpp',
    'Bengine/source/GLSLProgram.cpp',
    'Bengine/source/picopng.cpp',
    'Bengine/source/TextureCache.cpp',
    'Bengine/source/SpriteBatch.cpp',
    'Bengine/source/Camera.cpp',
    'Bengine/source/Input.cpp',
    'Bengine/source/Timing.cpp')


sdldep = dependency('sdl2', version : '>=2.30.7')
glewdep = dependency('glew', version : '>=2.2.0')

executable('zombie-game', srcdir,
    include_directories: incdir,
    dependencies: [sdldep, glewdep])

install_data('Textures/player.png', install_dir: meson.project_build_root() / 'Textures')
install_data('Textures/human.png', install_dir: meson.project_build_root() / 'Textures')
install_data('Textures/zombie.png', install_dir: meson.project_build_root() / 'Textures')
install_data('Textures/wood.png', install_dir: meson.project_build_root() / 'Textures')
install_data('Textures/red_bricks.png', install_dir: meson.project_build_root() / 'Textures')
install_data('Shaders/FragmentShader', install_dir: meson.project_build_root() / 'Shaders')
install_data('Shaders/VertexShader', install_dir: meson.project_build_root() / 'Shaders')

run_command(['mkdir', '-p', meson.project_build_root() / 'Textures'], check: true)

run_command(['cp', 'Textures/player.png', meson.project_build_root() / 'Textures/player.png'], check: true)
run_command(['cp', 'Textures/glass.png', meson.project_build_root() / 'Textures/glass.png'], check: true)
run_command(['cp', 'Textures/zombie.png', meson.project_build_root() / 'Textures/zombie.png'], check: true)
run_command(['cp', 'Textures/human.png', meson.project_build_root() / 'Textures/human.png'], check: true)
run_command(['cp', 'Textures/bullet.png', meson.project_build_root() / 'Textures/bullet.png'], check: true)

run_command(['cp', 'Textures/red_bricks.png', meson.project_build_root() / 'Textures/red_bricks.png'], check: true)
run_command(['cp', 'Textures/wood.png', meson.project_build_root() / 'Textures/wood.png'], check: true)

run_command(['mkdir', '-p', meson.project_build_root() / 'Shaders'], check: true)
run_command(['cp', 'Shaders/FragmentShader', meson.project_build_root() / 'Shaders/FragmentShader'], check: true)
run_command(['cp', 'Shaders/VertexShader', meson.project_build_root() / 'Shaders/VertexShader'], check: true)
