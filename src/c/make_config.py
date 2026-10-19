# make_config.py: writes config.bin = [key length][key][RC4(config)]
KEY = b"esrever"
CONFIG = b"host=c2.example.test;port=8443;id=lab-0042;sleep=60;mutex=Global\\LabMtx"

def rc4(key, data):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) & 0xFF
        S[i], S[j] = S[j], S[i]
    i = j = 0
    out = bytearray()
    for b in data:
        i = (i + 1) & 0xFF
        j = (j + S[i]) & 0xFF
        S[i], S[j] = S[j], S[i]
        out.append(b ^ S[(S[i] + S[j]) & 0xFF])
    return bytes(out)

if __name__ == "__main__":
    assert rc4(b"Key", b"Plaintext").hex() == "bbf316e8d940af0ad3"
    open("config.bin", "wb").write(bytes([len(KEY)]) + KEY + rc4(KEY, CONFIG))
