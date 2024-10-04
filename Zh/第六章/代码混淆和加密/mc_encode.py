# 原始字符串
input_string = " 1a cf 6d c1 72 6c d7 ae b6 0f a4 bd 7a 2a 31 28 86 65 0d 03 3a 72 4a e4 06 04 46 8d 54 53 5b cb de d9 84 0e 30 d3 36 f9 b5 4d d4 23 12 c4 f7 83 fc da 0d 7c 1a 92 b8 4d 12 8e 88 4f 66 5b f1 38 6f 4a ed e4 83 b1 05 43 5f ce 5a 35 b1 79 00 17 1d b5 20 5d 33 d3 66 ca 8e c7 d4 ad 2a 93 15 99 f2 c8 c4 44 f2 e3 f6 fa b6 e7 7a 99 91 cb 20 c0 77 87 1f 29 5a 9c f1 9f af 24 80 85 42 3a a6 f4 57 ce 24 94 c2 bf e9 10 17 52 65 3c 3b d3 00 9c a7 89 90 d6 be e7 10 44 f7 de e1 bb b2 a5 14 92 06 43 05 04 32 15 b6 70 35 b3 4c a3 9e c0 80 55 7f 16 6c 0b 93 a8 fc e9 e6 6e a4 8c 92 ba 68 27 7f 9d 6d 3d 83 8a 29 cb d6 9c 08 dd fb f9 5f 49 4e 36 c5 cf 8c cb 53 d3 67 86 ab d2 55 06 2e 91 8b aa 22 c4 a2 33 09 4b 32 08 e2 5a 65 de c9 ce 37 d7 7c f5 92 dd f0 ac fb 21 35 8d 8a 85 bc b4 63 98 ed 25 82 db 19 98 1f 65 e1 3e 2c 6c 9f bc 9e 06 95 14 c8 b3 2d 23 18 70 c1 a5 03 37 99 b9 a7 8a ec d6 0c 9c 7f c6 51 53 e2 1d 1d a3 2f ba c2 b7 6b da 3e c5 58 ad 4f 8d f4 38 39 1e c2 ca 94 69 c8 89 2e 53 78 81 8e 0f f4 ed aa bb b0 b9"

# 分割字符串并添加\x
output_string = ''.join([f'\\x{byte}' for byte in input_string.split()])

print(output_string)
