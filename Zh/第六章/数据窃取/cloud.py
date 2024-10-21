import os
from baidubce.bce_client_configuration import BceClientConfiguration
from baidubce.auth.bce_credentials import BceCredentials
from baidubce.services.bos.bos_client import BosClient
import oss2
from qcloud_cos import CosConfig, CosS3Client
from botocore.exceptions import NoCredentialsError

# 上传到百度云
def upload_to_baidu(file_name, bucket_name, object_name=None):
    config = BceClientConfiguration(credentials=BceCredentials('your-access-key-id', 'your-secret-access-key'), endpoint='http://bj.bcebos.com')
    client = BosClient(config)
    client.put_object_from_file(bucket_name, object_name or file_name, file_name)
    print("Upload Successful")

# 上传到阿里云
def upload_to_aliyun(file_name, bucket_name, object_name=None):
    auth = oss2.Auth('your-access-key-id', 'your-secret-access-key')
    bucket = oss2.Bucket(auth, 'http://oss-cn-hangzhou.aliyuncs.com', bucket_name)
    bucket.put_object_from_file(object_name or file_name, file_name)
    print("Upload Successful")

# 上传到腾讯云
def upload_to_tencent(file_name, bucket_name, object_name=None):
    secret_id = 'your-secret-id'
    secret_key = 'your-secret-key'
    region = 'ap-guangzhou'  # 替换为你选择的区域
    token = None  # 使用临时密钥需要传入Token，默认为None

    config = CosConfig(Region=region, SecretId=secret_id, SecretKey=secret_key, Token=token)
    client = CosS3Client(config)
    client.upload_file(Bucket=bucket_name, LocalFilePath=file_name, Key=object_name or file_name)
    print("Upload Successful")

def main():
    print("Select cloud storage service:")
    print("1. Baidu Cloud")
    print("2. Aliyun (Alibaba Cloud)")
    print("3. Tencent Cloud")
    choice = input("Enter choice (1/2/3): ")

    file_path = input("Enter path of the file to upload: ")

    if choice == '1':
        bucket_name = input("Enter Baidu Cloud bucket name: ")
        upload_to_baidu(file_path, bucket_name)
    elif choice == '2':
        bucket_name = input("Enter Aliyun bucket name: ")
        object_name = input("Enter object name: ")
        upload_to_aliyun(file_path, bucket_name, object_name)
    elif choice == '3':
        bucket_name = input("Enter Tencent Cloud bucket name: ")
        object_name = input("Enter object name: ")
        upload_to_tencent(file_path, bucket_name, object_name)
    else:
        print("Invalid choice")

if __name__ == "__main__":
    main()
