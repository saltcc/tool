from setuptools import setup, find_packages, Extension
import glob
import os.path
import sys


# C_SRC_PREFIX = os.path.join('cbits', 'webrtc', 'common_audio')
# C_SRC_RTC_PREFIX = os.path.join('cbits', 'webrtc', 'rtc_base')

# c_sources = (
#     [os.path.join(
#         'cbits', 'pywebrtcvad.c')]
#     + glob.glob(
#         os.path.join(
#             C_SRC_PREFIX,
#             'signal_processing',
#             '*.c'))
#     + glob.glob(
#         os.path.join(
#             C_SRC_PREFIX,
#             'third_party',
#             '*.c'))
#     + glob.glob(
#         os.path.join(
#             C_SRC_PREFIX,
#             'vad',
#             '*.c'))
#     + glob.glob(
#         os.path.join(
#             C_SRC_RTC_PREFIX,
#             'checks.cc')))

c_sources = ['tick_timer.cc', 'pydetector.c', 'detector.cc', 'delay_manager.cc', 'delay_peak_detector.cc']

define_macros = []
extra_compile_args = []

if sys.platform.startswith('win'):
    define_macros.extend([('_WIN32', None), ('WEBRTC_WIN', None)])
else:
    define_macros.extend([('WEBRTC_POSIX', None), ])
    extra_compile_args.extend(['-std=c++11'])

module = Extension('_pydetector',
                   define_macros=define_macros,
                   extra_compile_args=extra_compile_args,
                   sources=c_sources,
                   include_dirs=['./'])

# here = os.path.abspath(os.path.dirname(__file__))

# Get the long description from the README file
# with open(os.path.join(here, 'README.rst')) as f:
#     long_description = f.read()

setup(
    name='pydetector',
    author='John Wiseman',
    author_email='jjwiseman@gmail.com',
    version='2.0.11.dev0',
    description=('Python interface to the Google WebRTC Voice '
                 'Activity Detector (VAD)'),
    long_description='long_description',
    url='https://github.com/wiseman/py-webrtcvad',
    license='MIT',
    classifiers=[
        'Development Status :: 4 - Beta'
    ],
    # extra_link_args=['-lg++'],
    keywords='speechrecognition asr voiceactivitydetection vad webrtc',
    ext_modules=[module],
    py_modules=['pydetector'],
    test_suite='nose.collector',
    # List additional groups of dependencies here (e.g. development
    # dependencies). You can install these using the following syntax,
    # for example: $ pip install -e .[dev,test]
    extras_require={
        'dev': ['nose', 'check-manifest', 'unittest2', 'zest.releaser',
                'psutil', 'memory_profiler']
    })
