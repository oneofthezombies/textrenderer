from collections import deque
from itertools import chain
from pathlib import Path
import subprocess
import shutil
import os

os.chdir('harfbuzz-3.1.2')

with Path('build') as build_path:
    if build_path.exists():
        shutil.rmtree(build_path)

subprocess.run([
    'cmake',
    '-Bbuild',
    '-DCMAKE_BUILD_TYPE=Release',
    '-DBUILD_SHARED_LIBS=true',
    '-DCMAKE_APPLE_SILICON_PROCESSOR=arm64',
    '-DHB_HAVE_FREETYPE=true',
    '-DFREETYPE_LIBRARY=/Users/hunhoekim/repo/textrenderer/third_party/freetype/lib/macos/arm64/libfreetype.6.18.1.dylib',
    '-DFREETYPE_INCLUDE_DIRS=/Users/hunhoekim/repo/textrenderer/third_party/freetype/include'
    ]).check_returncode()
subprocess.run(['cmake', '--build', 'build']).check_returncode()

with Path('src') as src, Path('/Users/hunhoekim/repo/textrenderer/third_party/harfbuzz/include') as dest:
    if dest.exists():
        shutil.rmtree(dest)
    dest.mkdir()
    
    patterns = ['**/*.h', '**/*.hh']
    globs = map(src.glob, patterns)
    glob = chain.from_iterable(globs)
    paths = map(lambda path: (path, dest / path.relative_to(src)), glob)
    deque(map(lambda src_dst: shutil.copy(src_dst[0], src_dst[1]), paths))
    
with Path('build') as src, Path('/Users/hunhoekim/repo/textrenderer/third_party/harfbuzz/lib/macos/arm64') as dest:
    if dest.exists():
        shutil.rmtree(dest)
    dest.mkdir()
    
    glob = src.glob('**/*.dylib')
    paths = map(lambda path: (path, dest / path.relative_to(src)), glob)
    deque(map(lambda src_dst: shutil.copy(src_dst[0], src_dst[1]), paths))
