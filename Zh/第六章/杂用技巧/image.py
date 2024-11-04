from Crypto.Cipher import ARC4
from PIL import Image
import struct

def embed_payload(image_path, payload, output_path, keys):
    img = Image.open(image_path)
    img_data = bytearray(img.tobytes())

    //将有效载荷拆分成多个部分
    parts = [payload[i:i+16] for i in range(0, len(payload), 16)]

    //确保IDAT块数量与密钥数量匹配
    if len(parts) > len(keys):
        raise ValueError("Number of payload parts exceeds number of keys")

    //使用RC4加密每个部分并嵌入图片
    for i, part in enumerate(parts):
        cipher = ARC4.new(keys[i])
        encrypted_part = cipher.encrypt(part)
        start = len(img_data) - 512 * (i + 1)  # 假设每个部分不超过 512 字节
        img_data[start:start+len(encrypted_part)] = encrypted_part

    new_img = Image.frombytes(img.mode, img.size, bytes(img_data))
    new_img.save(output_path)

image_path = 'ghostwolflab.png'  //原图片地址
payload = b'This is the malicious payload'  //有效载荷
output_path = 'ghostwolf.png'  //输出图片
keys = [b'1234567890abcdef'] * 10  //密钥
embed_payload(image_path, payload, output_path, keys)
