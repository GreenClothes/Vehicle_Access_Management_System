ka = [b'\x1a', b'\x0a', b'\x0b', b'\x0b', b'\x0a', b'\x0a', b'\x0a', b'\x12']
na = [b'\x12', b'\x12', b'\x13', b'\x13', b'\x12', b'\x1e', b'\x02', b'\x02']
da = [b'\x02', b'\x1a', b'\x13', b'\x13', b'\x12', b'\x1e', b'\x02', b'\x02']
ra = [b'\x1a', b'\x0a', b'\x0b', b'\x1b', b'\x12', b'\x12', b'\x1a', b'\x02']
ma = [b'\x02', b'\x1a', b'\x1b', b'\x1b', b'\x1a', b'\x1a', b'\x1a', b'\x02']
ba = [b'\x16', b'\x16', b'\x1f', b'\x16', b'\x16', b'\x16', b'\x1e', b'\x02']
sa = [b'\x0a', b'\x0a', b'\x17', b'\x16', b'\x16', b'\x16', b'\x16', b'\x02']
a = [b'\x0a', b'\x16', b'\x17', b'\x16', b'\x16', b'\x16', b'\x0a', b'\x02']
ja = [b'\x1e', b'\x0a', b'\x17', b'\x16', b'\x16', b'\x16', b'\x16', b'\x02']
ha = [b'\x0a', b'\x1e', b'\x0b', b'\x16', b'\x16', b'\x16', b'\x0a', b'\x02']

keo = [b'\x1d', b'\x05', b'\x07', b'\x05', b'\x05', b'\x05', b'\x09', b'\x11']
neo = [b'\x11', b'\x11', b'\x17', b'\x11', b'\x11', b'\x11', b'\x1d', b'\x01']
deo = [b'\x1d', b'\x11', b'\x17', b'\x11', b'\x11', b'\x11', b'\x1d', b'\x01']
reo = [b'\x1d', b'\x05', b'\x07', b'\x1d', b'\x11', b'\x11', b'\x1d', b'\x01']
meo = [b'\x1d', b'\x15', b'\x17', b'\x15', b'\x15', b'\x15', b'\x1d', b'\x01']
beo = [b'\x15', b'\x15', b'\x1f', b'\x15', b'\x15', b'\x15', b'\x1d', b'\x01']
seo = [b'\x09', b'\x09', b'\x17', b'\x15', b'\x15', b'\x15', b'\x15', b'\x01']
eo = [b'\x09', b'\x15', b'\x17', b'\x15', b'\x15', b'\x15', b'\x09', b'\x01']
jeo = [b'\x1d', b'\x09', b'\x17', b'\x15', b'\x15', b'\x15', b'\x15', b'\x01']
heo = [b'\x09', b'\x1d', b'\x09', b'\x17', b'\x15', b'\x15', b'\x09', b'\x01']

ko = [b'\x1f', b'\x01', b'\x01', b'\x01', b'\x05', b'\x05', b'\x04', b'\x1f']
ho = [b'\x04', b'\x1f', b'\x04', b'\x0a', b'\x04', b'\x04', b'\x04', b'\x1f']

bae = [b'\x15', b'\x15', b'\x1d', b'\x17', b'\x15', b'\x1d', b'\x05', b'\x05']

def LICENSE_PLATE_CHAR(char):
    if char == '가': return b'\x00'
    elif char == '나': return b'\x01'
    elif char == '다': return b'\x02'
    elif char == '라': return b'\x03'
    elif char == '마': return b'\x04'
    elif char == '바': return b'\x05'
    elif char == '사': return b'\x06'
    elif char == '아': return b'\x07'
    elif char == '자': return b'\x08'
    elif char == '하': return b'\x09'
    elif char == '거': return b'\x0a'
    elif char == '너': return b'\x0b'
    elif char == '더': return b'\x0c'
    elif char == '러': return b'\x0d'
    elif char == '머': return b'\x0e'
    elif char == '버': return b'\x0f'
    elif char == '서': return b'\x10'
    elif char == '어': return b'\x11'
    elif char == '저': return b'\x12'
    elif char == '허': return b'\x13'
    elif char == '고': return b'\x14'
    elif char == '호': return b'\x15'
    elif char == '배': return b'\x16'
    return 1