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
    if plt == 'Linux':
        logging.info('using pkg-config and -ldl -lpthread -lm')
        deps += pkg_config('glfw3') + ' '
        deps += pkg_config('glew') + ' '
        deps += pkg_config('x11')
        deps += ' -ldl'
        deps += ' -lpthread'
        deps += ' -lm'
    elif plt == 'Darwin':
        logging.info('using pkg-config and manual -framework OpenGL')
        deps += pkg_config('glfw3') + ' '
        deps += pkg_config('glew') + ' '
        deps += ' -framework OpenGL'

    logging.info('all deps: %s', deps)

    Makefile = '''
libs = {}
game: main.o map.o fileio.o shader.o include
	g++ -o game main.o map.o fileio.o shader.o $(libs)

main.o: src/main.cpp
	g++ -c src/main.cpp -I./include -Wall
map.o: src/map.cpp
	g++ -c src/map.cpp -I./include -Wall
fileio.o: src/fileio.cpp
	g++ -c src/fileio.cpp -Wall
shader.o: src/shader.cpp
	g++ -c src/shader.cpp -Wall

clean:
	rm *.o
'''.format(deps).strip()

    logging.info('Makefile content:\n%s', Makefile)

    with open('Makefile', 'w') as f:
        f.write(Makefile)

