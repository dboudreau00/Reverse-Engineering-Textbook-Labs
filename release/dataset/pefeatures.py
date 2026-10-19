"""Three static features of a PE file, used by Labs 12.5 and 12.6."""
import math
from collections import Counter

import pefile


def entropy(data):
    n = len(data)
    return max(0.0, -sum(c / n * math.log2(c / n) for c in Counter(data).values())) if n else 0.0


def features(path):
    pe = pefile.PE(path)
    big = [s for s in pe.sections if s.SizeOfRawData >= 1024]
    max_entropy = max((entropy(s.get_data()) for s in big), default=0.0)
    imports = sum(len(d.imports) for d in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []))
    # a section that is large in memory but empty on disk is where unpacked code will go
    empty_in_file = max((s.Misc_VirtualSize / max(s.SizeOfRawData, 512) for s in pe.sections), default=0.0)
    return [max_entropy, float(imports), math.log2(1 + empty_in_file)]


NAMES = ["max section entropy", "imported functions", "log2(1 + virtual/raw)"]
