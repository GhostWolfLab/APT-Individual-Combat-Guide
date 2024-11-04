from Crypto.Cipher import ARC4
from PIL import Image

def extract_payload(image_path, keys):
    img = Image.open(image_path)
    img_data = bytearray(img.tobytes())
    parts = []

    //提取并解密每个部分
    for i in range(len(keys)):
        start = len(img_data) - 512 * (i + 1)
        encrypted_part = img_data[start:start+16]
        cipher = ARC4.new(keys[i])
        part = cipher.decrypt(bytes(encrypted_part))
        parts.append(part)

    payload = b''.join(parts)
    return payload

image_path = 'ghostwolf.png'  //图片地址
keys = [b'1234567890abcdef'] * 10  //密钥
payload = extract_payload(image_path, keys)
print(f'Extracted payload: {payload}')
