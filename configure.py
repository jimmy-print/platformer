#!/usr/bin/env python3

import platform
import logging
import subprocess

def pkg_config(pkg):
    proc = subprocess.run(['pkg-config', '--cflags', '--libs', pkg], capture_output=True)
    if proc.returncode != 0:
        raise RuntimeError('\'%s\' pkg does not exist' % pkg)
    config = proc.stdout.decode().strip()
    logging.info('pkg-config output: %s', config)
    return config

if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    plt = platform.system()
    logging.info('system is %s', plt)

    deps = ''
    misc = '-Iinclude -Wall'
    if plt == 'Linux':
        logging.info('using pkg-config and -ldl -lpthread -lm')
        deps += pkg_config('freetype2') + ' '
        deps += pkg_config('glfw3') + ' '
        deps += pkg_config('glew') + ' '
        deps += pkg_config('x11')
        deps += ' -ldl'
        deps += ' -lpthread'
        deps += ' -lm'
    elif plt == 'Darwin':
        logging.info('using pkg-config and manual -framework OpenGL')
        deps += pkg_config('freetype2') + ' '
        deps += pkg_config('glfw3') + ' '
        deps += pkg_config('glew') + ' '
        deps += ' -framework OpenGL'
    elif plt == 'Windows':
        logging.info('using manual everything')
        misc += ' -Llibs -DGLEW_STATIC'
        deps += '-lglew32s -lglfw3 -lopengl32'

    logging.info('all deps: %s', deps)

    Makefile = '''
objs = main.o bullet.o fileio.o map.o rect.o shader.o character.o text.o
misc = {}
libs = {}
platformer: $(objs)
	g++ -g -o platformer $(objs) $(misc) $(libs)

main.o: src/main.cpp
	g++ -g -c src/main.cpp $(misc) $(libs)
map.o: src/map.cpp
	g++ -g -c src/map.cpp $(misc)
fileio.o: src/fileio.cpp
	g++ -g -c src/fileio.cpp $(misc)
shader.o: src/shader.cpp
	g++ -g -c src/shader.cpp $(misc)
rect.o: src/rect.cpp
	g++ -g -c src/rect.cpp $(misc)
bullet.o: src/bullet.cpp
	g++ -g -c src/bullet.cpp $(misc)
character.o: src/character.cpp
	g++ -g -c src/character.cpp $(misc)
text.o: src/text.cpp
	g++ -g -c src/text.cpp $(misc) $(libs)
clean:
	rm *.o
'''.format(misc, deps).strip()

    logging.info('Makefile content:\n%s', Makefile)

    with open('Makefile', 'w') as f:
        f.write(Makefile)
