import pkg_resources

import _pydetector

# __author__ = "John Wiseman jjwiseman@gmail.com"
# __copyright__ = "Copyright (C) 2016 John Wiseman"
# __license__ = "MIT"
# __version__ = pkg_resources.get_distribution('pydetector').version


class WebDete(object):
    def __init__(self, length=None):
        self._detector = _pydetector.create()
        # _webrtcvad.init(self._vad)
        if length is not None:
            self.set_audio_length(length)

    def set_audio_length(self, length):
        _pydetector.set_audio_length(self._detector, length)

    def update(self, seq, timestamp, hz):
        return _pydetector.update(self._detector, seq, timestamp, hz)

    def tick(self, x):
        _pydetector.tick(self._detector, x)

    def target_level(self):
        return _pydetector.target_level(self._detector)
    
    def histogram(self):
        return _pydetector.histogram(self._detector)