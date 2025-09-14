import math
import struct

M = 4
N = 10

with open("matrice.dat", "wb") as f:
    # Header
    f.write(struct.pack("<i", M))
    f.write(struct.pack("<i", N))
    
    # Matrice
    for m in range(M):
        for i in range(N+1):
            val = 0.0 if i == 0 else i * math.ceil(math.log(i)) + (m*m % 17)
            f.write(struct.pack("<d", val))
