import boto3
from botocore.exceptions import NoCredentialsError
from google.cloud import storage
from azure.storage.blob import BlobServiceClient
import os

def upload_to_s3(file_name, bucket, object_name=None):
    s3_client = boto3.client('s3')
    try:
        s3_client.upload_file(file_name, bucket, object_name or file_name)
        print("Upload Successful")
    except NoCredentialsError:
        print("Credentials not available")

def upload_to_gcs(bucket_name, source_file_name, destination_blob_name):
    storage_client = storage.Client()
    bucket = storage_client.bucket(bucket_name)
    blob = bucket.blob(destination_blob_name)
    blob.upload_from_filename(source_file_name)
    print("Upload Successful")

def upload_to_azure_blob(container_name, file_name, blob_name):
    connect_str = "your_connection_string"
    blob_service_client = BlobServiceClient.from_connection_string(connect_str)
    blob_client = blob_service_client.get_blob_client(container=container_name, blob=blob_name)
    with open(file_name, "rb") as data:
        blob_client.upload_blob(data)
    print("Upload Successful")

def main():
    print("Select cloud storage service:")
    print("1. Amazon S3")
    print("2. Google Cloud Storage")
    print("3. Azure Blob Storage")
    choice = input("Enter choice (1/2/3): ")

    file_path = input("Enter path of the file to upload: ")

    if choice == '1':
        bucket = input("Enter S3 bucket name: ")
        upload_to_s3(file_path, bucket)
    elif choice == '2':
        bucket = input("Enter GCS bucket name: ")
        blob_name = input("Enter blob name: ")
        upload_to_gcs(bucket, file_path, blob_name)
    elif choice == '3':
        container = input("Enter Azure Blob container name: ")
        blob_name = input("Enter blob name: ")
        upload_to_azure_blob(container, file_path, blob_name)
    else:
        print("Invalid choice")

if __name__ == "__main__":
    main()
