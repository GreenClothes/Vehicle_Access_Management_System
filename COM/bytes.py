import struct

test1 = bytes(1)
test2 = bytes([0x1a, 0x0a, 0x0b, 0x0b, 0x0a, 0x0a, 0x0a, 0x12])
test5 = [b'\xff', b'\x10', b'\x01']
test3 = bytes(b'\xff')
test4 = []
for i in range(len(test5)):
    test4.append(bytes(test5[i]))
#print(type(test4[1]))

b = b'\x0a'
b = struct.unpack('B', b)
c = b[0] + 3
c = struct.pack('B', c)
print(c)

