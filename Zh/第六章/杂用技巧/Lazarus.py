#!usr/bin/python3
from uuid import UUID
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-p','--payload', required = True, help = "payload: Binary File")
args = vars(parser.parse_args())
pbin = args['payload']

with open(pbin, "rb") as f:
    chunk = f.read(16)
    while chunk:
        if len(chunk) < 16:
            padding = 16 - len(chunk)
            chunk = chunk + (b"\x90" * padding)
        print(UUID(bytes_le=chunk))
        chunk = f.read(16)
